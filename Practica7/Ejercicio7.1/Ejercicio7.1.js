"use strict";
class MapaSensores {
    constructor() {
        this.mapa; // el mapa de Google Maps
        this.sensores = []; // array para almacenar los sensores
        
        this.TIME_BETWEEN_SENSOR_MEASUREMENT = 3000; // tiempo entre cada medición a los sensores
        this.MAX_CORRECT_HUMIDITY = 36; // si algún sensor tiene esta humedad+1 se muestra una advertencia
    }

    // Crea el mapa de Google y los sensores (con su marcador en el mapa)
    inicializar() {
        // Creamos los sensores. Solo uno de ellos será de verdad. El resto tienen valores fijos, que no cambian.
        var sensorReal   = new Sensor('192.168.61.37', 'Escuela de Ingeniería Informática', 43.3545734, -5.8511042, true, 0, 0, this);
        var sensorFalso1 = new Sensor('192.168.61.100', 'Facultad de Profesorado y Educación', 43.357738, -5.855048, false, 20, 32, this); 
        var sensorFalso2 = new Sensor('192.168.61.101', 'Facultad de Ciencias', 43.357936, -5.853565, false, 22, 30, this); 
        var sensorFalso3 = new Sensor('192.168.61.102', 'Palacio de Congresos Príncipe Felipe', 43.357514, -5.851170, false, 25, 31, this);
        
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
            '<div class="infoWindowContent">' + 
                '<div id="mensajeError'+ sensor.IP +'"></div>' +
                '<div>' +
                    '<h2>' + sensor.nombre + '</h2>' +
                    '<p>Latitud: ' + sensor.latitud + '</p>' +
                    '<p>Longitud: ' + sensor.longitud + '</p>' +
                    '<p>Temperatura: <span id="temperatura'+ sensor.IP +'">' + sensor.temperatura + '</span></p>' +
                    '<p>Humedad: <span id="humedad'+ sensor.IP +'">' + sensor.humedad + '</span></p>' +
                    '<input type="button" id="botonEncender'+ sensor.IP +'" class="button button-green" ' +
                        'onclick="mapa.encenderLed(\''+ sensor.IP +'\', true);" value="Encender" />' +
                    '<input type="button" id="botonApagar'+ sensor.IP +'" class="button button-red" ' + 
                        'onclick="mapa.encenderLed(\''+ sensor.IP +'\', false);" value="Apagar" />' +
                '</div>' +
            '</div>';

        var infowindow = new google.maps.InfoWindow({
            content: contentString
        });

        // Añadimos un escuchador del evento click para cuando se pulse ese marcador
        marcador.addListener('click', this.clickInfoWindowHandler.bind(this, marcador, infowindow, sensor));
        
        // Abrimos y cerramos el infowindow para que se creee sus id.
        infowindow.open(this.mapa, marcador);
        infowindow.close();
        
        return marcador;
    }
    
    // Para cada uno de los sensores, consulta y almacena la temperatura y la humedad
    consultarSensores(){
        console.log('\nSe consulta la información de los sensores');
        
        for (var i = 0; i < this.sensores.length; i++) {
            this.sensores[i].consultarInformacion();
        }
    }
    
    // Cuando se hace click en un marcador, se abre su infoWindow correspondiente
    // y se actualiza la información mostrada
    clickInfoWindowHandler(marcador, infowindow, sensor) {
        infowindow.open(this.mapa, marcador);
        sensor.mostrarInformacion(); // Mostramos los datos del sensor que teniamos almacenados previamente
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
        var mensaje = '¡Aviso! Humedad más alta de ' + this.MAX_CORRECT_HUMIDITY +
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
    
    // Le dice al sensor con la IP dada que encienda/apague su led
    encenderLed(sensorIP, encender) {
        // Buscamos en el array la referencia al led con esa IP
        for (var i = 0; i < this.sensores.length; i++) {
            if (this.sensores[i].IP == sensorIP) {
                // En función de la variable booleana encender, se enciende o apaga el led
                if(encender)
                    this.sensores[i].encenderLed();
                else
                    this.sensores[i].apagarLed();
                
                break;
            }
        }
    }
    
}

"use strict";
class Sensor {
    constructor(IP, nombre, latitud, longitud, isReal, temperatura, humedad, mapa) {
        this.IP = IP; // actua de identificador único
        this.nombre = nombre; // nombre/título para el sensor
        this.latitud = latitud;
        this.longitud = longitud;
        this.isReal = isReal; // indica si el sensor es real o no
        this.temperatura = temperatura; // almacena la temperatura del sensor (valor fijo para los sensores falsos)
        this.humedad = humedad; // almacena la humedad del sensor (valor fijo para los sensores falsos)
        this.mensajeError; // almacena un mensaje de error al consultar la información del sensor
        
        this.mapa = mapa; // referencia al mapa en el que se encuentra el sensor
        this.marcador; // almacena el marcador de Google para ese sensor
        
        this.RANDOM_TEMPERATURE_MIN_VALUE = 17; // tiempo mínimo para la temperatura aleatoria de los sensores falsos
        this.RANDOM_TEMPERATURE_MAX_VALUE = 26; // tiempo mínimo para la temperatura aleatoria de los sensores falsos
        this.RANDOM_HUMIDITY_MIN_VALUE = 20; // tiempo mínimo para la temperatura aleatoria de los sensores falsos
        this.RANDOM_HUMIDITY_MAX_VALUE = 30; // tiempo mínimo para la temperatura aleatoria de los sensores falsos
    }
    
    // Obtiene la temperatura y la humedad del sensor, si es real,
    // o la genera aleatoriamente, si es falso. En ambos casos, almacena esa información.
    consultarInformacion(){
        if(this.isReal)
            this.consultarInformacionReal();
        else
            this.consultarInformacionFalsa();
    }
    
    // Consulta la temperatura y la humedad utilizando Ajax con JQuery,
    // y si tiene éxito, actualiza la información del sensor.
    consultarInformacionReal() {
        // Se realiza una petición al sensor, pidiendole la temperatura y la humedad
        var urlSensor = 'http://' + this.IP + '/temperaturayhumedad';

        $.ajax({
            context: this,
            dataType: 'json',
            url: urlSensor,
            method: 'GET',
            success: function (data) {
                // Si tiene éxito, se actualiza la temperatura y humedad del sensor
                this.actualizarTemperaturaYHumedad(data.temperatura, data.humedad);
                this.mensajeError = null; // se pone a null el mensaje de error, para indicar que no se debe mostrar nada
            },
            error: function () {
                // Si no tiene éxito, se actualiza el mensaje de error
                this.mensajeError = '¡En este momento no se puede obtener la información de la temperatura y la humedad!';
            }
        });
    }
    
    // Genera aleatoriamente en un rango la temperatura y la humedad, y las almacena.
    consultarInformacionFalsa(){
        var temperaturaAleatoria = this.randomIntFromInterval(this.RANDOM_TEMPERATURE_MIN_VALUE, 
                                                         this.RANDOM_TEMPERATURE_MAX_VALUE);
        var humedadAleatoria = this.randomIntFromInterval(this.RANDOM_HUMIDITY_MIN_VALUE, 
                                                     this.RANDOM_HUMIDITY_MAX_VALUE);
        
        // Se actualiza la información del sensor
        this.actualizarTemperaturaYHumedad(temperaturaAleatoria, humedadAleatoria);
    }
    
    // Almacena la temperatura y la humedad
    actualizarTemperaturaYHumedad(temperatura, humedad){
        console.log('Valores del sensor ' + this.nombre + 
                    ": Temperatura: " + temperatura + 
                    ": Humedad: " + humedad);
        
        this.temperatura = temperatura;
        this.humedad = humedad;
        
        // Se actualiza el mensaje del estado de los sensores, 
        // con la nueva información almacenada de este sensor
        this.mapa.actualizarMensajeEstadoSensores();
    }
    
    // Actualiza el infowindow con la información del sensor
    mostrarInformacion(){
        // Se actualiza el infowindow correspondiente al sensor
        document.getElementById('temperatura' + this.IP).innerHTML = this.temperatura.toString();
        document.getElementById('humedad' + this.IP).innerHTML = this.humedad.toString();
        
        // Si hay un mensaje de error, lo mostramos, y si no, quitamos el mensaje de error
        if(this.mensajeError != null) 
            document.getElementById('mensajeError' + this.IP).innerHTML = this.mensajeError;
        else
            document.getElementById('mensajeError' + this.IP).innerHTML = '';
    }
    
    // Enciende el led del sensor, mandando una petición utilizando Ajax con JQuery.
    encenderLed(){
        console.log("Se va a intentar encender la luz del sensor: ");
        console.log(this);
        
        // Se realiza una petición al sensor, pidiendole que encienda el led
        var urlSensor = 'http://' + this.IP + '/led/encender';

        $.ajax({
            context: this,
            dataType: 'json',
            url: urlSensor,
            method: 'GET',
            success: function () {
                console.log("Encendida correctamente la luz led del sensor " + this.nombre);
            },
            error: function () {
                console.log("No se pudo encender la luz led del sensor " + this.nombre);
            }
        });
    }
    
    // Apagar el led del sensor, mandando una petición utilizando Ajax con JQuery.
    apagarLed(){
        console.log("Se va a intentar apagar la luz del sensor: ");
        console.log(this);
        
        // Se realiza una petición al sensor, pidiendole que apague el led
        var urlSensor = 'http://' + this.IP + '/led/apagar';

        $.ajax({
            context: this,
            dataType: 'json',
            url: urlSensor,
            method: 'GET',
            success: function () {
                console.log("Apagada correctamente la luz led del sensor " + this.nombre);
            },
            error: function () {
                console.log("No se pudo apagar la luz led del sensor " + this.nombre);
            }
        });
    }
    
    // Genera un número aleatorio en el rango [min,max] (ambos inclusive)
    randomIntFromInterval(min, max){
        return Math.floor(Math.random() * (max - min + 1) + min);
    }
    
}


// Se crea el mapa
var mapa = new MapaSensores();
google.maps.event.addDomListener(window, 'load', mapa.inicializar.bind(mapa));
