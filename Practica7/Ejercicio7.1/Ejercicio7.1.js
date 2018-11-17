"use strict";
class MapaSensores {
    constructor() {
        this.mapa; // el mapa de Google Maps
        this.sensores = []; // array para almacenar los sensores
        
        this.TIME_BETWEEN_SENSOR_MEASUREMENT = 7000; // tiempo entre cada medición a los sensores
        this.RANDOM_TEMPERATURE_MIN_VALUE = 17; // tiempo mínimo para la temperatura aleatoria de los sensores falsos
        this.RANDOM_TEMPERATURE_MAX_VALUE = 26; // tiempo mínimo para la temperatura aleatoria de los sensores falsos
        this.RANDOM_HUMIDITY_MIN_VALUE = 20; // tiempo mínimo para la temperatura aleatoria de los sensores falsos
        this.RANDOM_HUMIDITY_MAX_VALUE = 30; // tiempo mínimo para la temperatura aleatoria de los sensores falsos
        
        this.MAX_CORRECT_HUMIDITY = 36; // si algún sensor tiene esta humedad+1 se muestra una advertencia
    }

    // Crea el mapa de Google y los sensores (con su marcador en el mapa)
    inicializar() {
        // Creamos los sensores. Solo uno de ellos será de verdad. El resto tienen valores fijos, que no cambian.
        var sensorReal   = new Sensor('192.168.61.37', 'Escuela de Ingeniería Informática', 43.3545734, -5.8511042, true, 0, 0);
        var sensorFalso1 = new Sensor('192.168.61.100', 'Facultad de Formación del Profesorado y Educación', 43.357738, -5.855048, false, 20, 32); 
        var sensorFalso2 = new Sensor('192.168.61.101', 'Facultad de Ciencias', 43.357936, -5.853565, false, 22, 30); 
        var sensorFalso3 = new Sensor('192.168.61.102', 'Palacio de Congresos Príncipe Felipe', 43.357514, -5.851170, false, 25, 31);
        
        // Metemos todos los sensores en el array de sensores
        this.sensores.push(sensorReal, sensorFalso1, sensorFalso2, sensorFalso3);
        
        // Inicializamos el mapa de Google
        this.mapa = new google.maps.Map(document.getElementById('mapa-canvas'), {
            center: {
                lat: sensorReal.latitud,
                lng: sensorReal.longitud
            },
            zoom: 15
        });

        // Añadimos el marcador con infowindow para cada uno de los sensores
        for (var i = 0; i < this.sensores.length; i++) {
            this.setMarcadorConInfoWindow(this.sensores[i]);
        }
        
        // Añadimos el mensaje arriba de la página
        this.actualizarMensajeEstadoSensores();
        
        // Cada TIME_BETWEEN_SENSOR_MEASUREMENT milisegundos obtenemos los datos de los sensores
        setInterval(this.consultarSensores.bind(this), this.TIME_BETWEEN_SENSOR_MEASUREMENT);
    }
    
    // Establece el marcador con infowindow para el sensor
    setMarcadorConInfoWindow(sensor){
        // Creamos el marcador
        var marcador = new google.maps.Marker({
            position: new google.maps.LatLng(sensor.latitud, sensor.longitud),
            map: this.mapa,
            title: sensor.nombre
        });
        
        // Le añadimos la infowindow
        marcador = this.addInfoWindow(marcador, sensor);
        
        // Al sensor se le asocia su marcador
        sensor.marcador = marcador;
    }
    
    // Añade un infowindow al marcador indicado, y con la información del sensor
    addInfoWindow(marcador, sensor) {
        //Creamos una infoWindow con esta estructura html
        var contentString = 
            '<div id="mensajeError'+ sensor.IP +'"></div>' +
            '<div>' +
                '<h2>' + sensor.nombre + '</h2>' +
                '<p>Latitud: ' + sensor.latitud + '</p>' +
                '<p>Longitud: ' + sensor.longitud + '</p>' +
                '<p>Temperatura:<span id="temperatura'+ sensor.IP +'">' + sensor.temperatura + '</span></p>' +
                '<p>Humedad:<span id="humedad'+ sensor.IP +'">' + sensor.humedad + '</span></p>' +
            '</div>';

        var infowindow = new google.maps.InfoWindow({
            content: contentString
        });

        // Añadimos un escuchador del evento click para cuando se pulse ese marcador
        marcador.addListener('click', this.clickInfoWindowHandler.bind(this, marcador, infowindow, sensor));
        
        return marcador;
    }
    
    // Para cada uno de los sensores, consulta y almacena la temperatura y la humedad
    consultarSensores(){
        console.log('\nSe consulta la información de los sensores');
        
        for (var i = 0; i < this.sensores.length; i++) {
            this.consultarSensor(this.sensores[i]);
        }
    }
    
    // Obtiene y almacena la temperatura y la humedad del sensor, si es real,
    // o la genera aleatoriamente, si es falso.
    consultarSensor(sensor) {
        if(sensor.isReal)
            this.consultarSensorReal(sensor);
        else
            this.consultarSensorFalso(sensor);
    }
    
    // Consulta al sensor indicado la temperatura y la humedad utilizando Ajax con JQuery,
    // y si tiene éxito, actualiza la información de ese sensor
    consultarSensorReal(sensor){
        // Se realiza una petición al sensor, pidiendole la temperatura y la humedad
        var urlSensor = 'http://' + sensor.IP + '/temperaturayhumedad';

        $.ajax({
            dataType: 'json',
            url: urlSensor,
            method: 'GET',
            success: function(data){
                // Si tiene éxito, se actualiza la temperatura y humedad del sensor
                this.actualizarTemperaturaYHumedadSensor(sensor, data.temperatura, data.humedad);
                sensor.mensajeError = null; // se pone a null el mensaje de error, para indicar que no se debe mostrar nada
            },
            error: function () {
                // Si no tiene éxito, se actualiza el mensaje de error
                sensor.mensajeError = '¡En este momento no se puede obtener la información de la temperatura y la humedad!';
            }
        });
    }
    
    // Genera aleatoriamente en un rango la temperatura y la humedad del sensor, y las almacena
    consultarSensorFalso(sensor){
        var temperaturaAleatoria = this.randomIntFromInterval(this.RANDOM_TEMPERATURE_MIN_VALUE, 
                                                         this.RANDOM_TEMPERATURE_MAX_VALUE);
        var humedadAleatoria = this.randomIntFromInterval(this.RANDOM_HUMIDITY_MIN_VALUE, 
                                                     this.RANDOM_HUMIDITY_MAX_VALUE);
        
        // Se actualiza la información del sensor
        this.actualizarTemperaturaYHumedadSensor(sensor, temperaturaAleatoria, humedadAleatoria);
    }
    
    // Almacena la temperatura y la humedad en el sensor indicado
    actualizarTemperaturaYHumedadSensor(sensor, temperatura, humedad){
        console.log('Valores del sensor ' + sensor.nombre + 
                    ": Temperatura: " + temperatura + 
                    ": Humedad: " + humedad);
        
        sensor.temperatura = temperatura;
        sensor.humedad = humedad;
        
        // Se actualiza el mensaje del estado de los sensores, 
        // con la nueva información almacenada de este sensor
        this.actualizarMensajeEstadoSensores();
    }
    
    // Cuando se hace click en un marcador, se abre su infoWindow correspondiente
    // y se actualiza la información mostrada
    clickInfoWindowHandler(marcador, infowindow, sensor) {
        infowindow.open(this.mapa, marcador);
        this.mostrarInformaciónSensor(sensor); // Mostramos los datos del sensor que teniamos almacenados previamente
    }
    
    // Actualiza el infowindow con la información del sensor
    mostrarInformaciónSensor(sensor){
        // Si aún no ha cargado el id dinámico del infowindow, no hacemos nada
        if(document.getElementById('temperatura' + sensor.IP) == null) // TODO - ver si se puede mejorar!!
            return;
        
        // Se actualiza el infowindow correspondiente a ese sensor
        document.getElementById('temperatura' + sensor.IP).innerHTML = sensor.temperatura.toString();
        document.getElementById('humedad' + sensor.IP).innerHTML = sensor.humedad.toString();
        
        // Si hay un mensaje de error, lo mostramos, y si no, quitamos el mensaje de error
        if(sensor.mensajeError != null) 
            document.getElementById('mensajeError' + sensor.IP).innerHTML = sensor.mensajeError;
        else
            document.getElementById('mensajeError' + sensor.IP).innerHTML = '';
    }
    
    // Genera un número aleatorio en el rango [min,max] (ambos inclusive)
    randomIntFromInterval(min, max){
        return Math.floor(Math.random() * (max - min + 1) + min);
    }
    
    // Actualiza el mensaje correspondiente al estado de los sensores
    actualizarMensajeEstadoSensores(){
        // Si hay algún sensor con humedad superior límite, se muestra una advertencia
        for (var i = 0; i < this.sensores.length; i++) {
            if(this.sensores[i].humedad > this.MAX_CORRECT_HUMIDITY){
                this.sensorConEstadoPeligroso(this.sensores[i]);
                return;
            }
        }
        
        // Si todos los sensores están bien, se muestra un mensaje de todos correctos.
        this.todosLosSensoresCorrectos();
    }
    
    // El sensor pasado como parámetro tiene un estado peligroso, 
    // por lo que se actualiza el mensaje en función de ello.
    sensorConEstadoPeligroso(sensor){
        var mensaje = '¡Aviso! Humedad más alta de ' + sensor.humedad +
            ' en ' + sensor.nombre + ' que está en (' + sensor.latitud +
            ', ' + sensor.longitud + ') con IP ' + sensor.IP;

        $('#mensajeEstadoSensores').html(mensaje);
        $('#mensajeEstadoSensores').css("background-color", "#FF908B");
    }
    
    // Todos los sensores están bien, por lo que se muestra un mensaje
    // de que todos están correctos.
    todosLosSensoresCorrectos(){
        var mensaje = 'Todos los sensores están correctos';

        $('#mensajeEstadoSensores').html(mensaje);
        $('#mensajeEstadoSensores').css("background-color", "#86FF94");
    }
    
}

"use strict";
class Sensor {
    constructor(IP, nombre, latitud, longitud, isReal, temperatura, humedad) {
        this.IP = IP; // actua de identificador único
        this.nombre = nombre; // nombre/título para el sensor
        this.latitud = latitud;
        this.longitud = longitud;
        this.isReal = isReal; // indica si el sensor es real o no
        this.temperatura = temperatura; // almacena la temperatura del sensor (valor fijo para los sensores falsos)
        this.humedad = humedad; // almacena la humedad del sensor (valor fijo para los sensores falsos)
        this.mensajeError; // almacena un mensaje de error al consultar la información del sensor
        
        this.marcador; // almacena el marcador de Google para ese sensor
    }
}


// Se crea el mapa
var mapa = new MapaSensores();
google.maps.event.addDomListener(window, 'load', mapa.inicializar.bind(mapa));