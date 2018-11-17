"use strict";
class MapaSensores {
    constructor() {
        this.mapa; // el mapa de Google Maps
        this.sensores = []; // array para almacenar los sensores
        //this.marcadores = []; //Array para guardar los marcadores // TODO - quitar
    }

    // Crea el mapa de Google y los sensores (con su marcador en el mapa)
    inicializar() {
        // Creamos los sensores.
        // Solo uno de ellos será un Arduino de verdad. El resto tienen valores fijos, que no cambian.
        var sensorReal   = new Sensor('192.168.61.37', 'Escuela de Ingeniería Informática', 43.3545734, -5.8511042, true, 0, 0);
        var sensorFalso1 = new Sensor('192.168.61.100', 'Facultad de Formación del Profesorado y Educación', 43.357738, -5.855048, false, 20, 32); 
        var sensorFalso2 = new Sensor('192.168.61.101', 'Facultad de Ciencias', 43.357936, -5.853565, false, 22, 30); 
        var sensorFalso3 = new Sensor('192.168.61.102', 'Palacio de Congresos Príncipe Felipe', 43.357514, -5.851170, false, 25, 40);
        
        // Inicializamos el mapa de Google
        this.mapa = new google.maps.Map(document.getElementById('mapa-canvas'), {
            center: {
                lat: sensorReal.latitud,
                lng: sensorReal.longitud
            },
            zoom: 15
        });
        
        // Metemos todos los sensores en el array de sensors
        this.sensores.push(sensorReal);
        this.sensores.push(sensorFalso1);
        this.sensores.push(sensorFalso2);
        this.sensores.push(sensorFalso3);

        // Añadimos el marcador con ventanita para cada uno de los sensores
        for (var i = 0; i < this.sensores.length; i++) {
            this.setMarcadorConInfoWindow(this.sensores[i]);
        }
        
        // TODO - quitar
        //this.addMarcadorConInfoWindow('Escuela Ingeniería Informatica', this.latitudArduino, this.longitudArduino, 0, 0);
        // Añadimos el resto de marcadores con 'sensores falsos'
        //this.addMarcadorConInfoWindow('Facultad de Magisterio', 43.357738, -5.855048, 20, 32);
        //this.addMarcadorConInfoWindow('Facultad de Ciencias ', 43.357936, -5.853565, 21, 30);
    }
    
    // Establece el marcador con ventanita para el sensor
    setMarcadorConInfoWindow(sensor){
        // Creamos el marcador
        var marcador = new google.maps.Marker({
            position: new google.maps.LatLng(sensor.latitud, sensor.longitud),
            map: this.mapa,
            title: sensor.nombre
        });
        
        // Le añadimos la ventanita
        marcador = this.addInfoWindow(marcador, sensor);
        
        // Al sensor se le asocia su marcador
        sensor.marcador = marcador;
    }
    
    // Añade un infowindow al marcador indicado, y con la información del sensor
    addInfoWindow(marcador, sensor) {
        //Creamos una infoWindow con esta estructura html
        var contentString = '<div id="mensajeError"></div>' +
            '<div>' +
            '<div>' +
            '<h2>' + sensor.nombre + '</h2>' +
            '</div>' +
            '<div id="datos">' +
            '<p>Latitud: ' + sensor.latitud + '</p>' +
            '<p>Longitud: ' + sensor.longitud + '</p>' +
            '<p>Temperatura:<span id="temperatura">' + sensor.temperatura + '</span></p>' +
            '<p>Humedad:<span id="humedad">' + sensor.humedad + '</span></p>' +
            '</div>' +
            '</div>';

        var infowindow = new google.maps.InfoWindow({
            content: contentString
        });

        // Añadimos un escuchador del evento click para cuando se pulse ese marcador
        marcador.addListener('click', this.clickInfoWindowHandler.bind(this, marcador, infowindow));
        
        return marcador;
    }
    
    // Cuando se hace click en un marcador, se abre su infoWindow correspondiente
    clickInfoWindowHandler(marcador, infowindow) {
        infowindow.open(this.mapa, marcador);
    }
    
    // Consulta al sensor indicado la temperatura y la humedad utilizando Ajax con JQuery,
    // y si tiene éxito, actualiza la temperatura y humedad en la web
    actualizarTemperaturaYHumedad(sensor) {
        // Si el sensor es falso, no se hace nada
        if(!sensor.isReal)
            return;
        
        // Se realiza una petición al sensor, pidiendole la temperatura y la humedad
        var urlSensor = 'http://' + sensor.IP + '/temperaturayhumedad';

        $.ajax({
            dataType: 'json',
            url: urlSensor,
            method: 'GET',
            success: function(data){
                // Si tiene exito, se actualiza el infowindow correspondiente a ese sensor
                $('#temperatura' + sensor.IP).html(data.temperatura);
                $('#humedad' + sensor.IP).html(data.humedad);
            },
            error: function () {
                $('#mensajeError' + sensor.IP).html('¡En este momento no se puede obtener la información de la temperatura y la humedad!');
            }
        });
    }

    
    /*
    // Crea un marcador con las coordenadas indicadas y le añade un infowindow con la información indicada
    addMarcadorConInfoWindow(lugar, latitud, longitud, temperatura, humedad) {
        var marcador = this.addMarcador(latitud, longitud, lugar);
        this.addInfoWindow(marcador, lugar, latitud, longitud, temperatura, humedad);
    }
    */

    // Crea un marcador con las coordenadas indicadas, le pone el title indicado en lugar,
    // lo añade al mapa, y lo almacena en el array de marcadores
    /*
    addMarcador(latitud, longitud, lugar) {
        var marcador = new google.maps.Marker({
            position: new google.maps.LatLng(latitud, longitud),
            map: this.mapa,
            title: lugar
        });
        this.marcadores.push(marcador);

        return marcador;
    }*/

    // Añade un infowindow al marcador indicado, y con la información indicada
    /*
    addInfoWindow(marcador, lugar, latitud, longitud, temperatura, humedad) {
        //Creamos una infoWindow con esta estructura html
        var contentString = '<div id="mensajeError"></div>' +
            '<div>' +
            '<div>' +
            '<h2>' + lugar + '</h2>' +
            '</div>' +
            '<div id="datos">' +
            '<p>Latitud: ' + latitud + '</p>' +
            '<p>Longitud: ' + longitud + '</p>' +
            '<p>Temperatura:<span id="temperatura">' + temperatura + '</span></p>' +
            '<p>Humedad:<span id="humedad">' + humedad + '</span></p>' +
            '</div>' +
            '</div>';

        var infowindow = new google.maps.InfoWindow({
            content: contentString
        });

        // Añadimos un escuchador del evento click para cuando se pulse ese marcador
        marcador.addListener('click', this.clickInfoWindowHandler.bind(this, marcador, infowindow));
    }*/

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
        
        this.marcador; // almacena el marcador de Google para ese sensor
    }
}


// Se crea el mapa
var mapa = new MapaSensores();
google.maps.event.addDomListener(window, 'load', mapa.inicializar.bind(mapa));