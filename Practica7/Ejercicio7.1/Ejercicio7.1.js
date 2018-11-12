"use strict";
class MapaSensores{
    constructor(){
        this.mapa; // el mapa de Google Maps
        
        this.marcadores=[]; //Array para guardar los marcadores
        
        this.latitudArduino = 43.3545734; // latitud del arduino, harcodeada
        this.longitudArduino = -5.8511042; // longitud del arduino, harcodeada
        this.IPArduino = "192.168.61.37"; // ip del Arduino
    }
    
    // Crea el mapa de Google y añade los marcadores correspondientes a los sensores
    inicializar() {
        // Inicializamos el mapa de Google
        this.mapa = new google.maps.Map(document.getElementById('mapa-canvas'), {
          center: {lat: this.latitudArduino, lng: this.longitudArduino},
          zoom: 15
        });
        
        console.log(this);
        console.log("MENSAJE");

        // Añadimos los marcadores correspondientes a los sensores.
        // Solo uno de ellos será un Arduino de verdad. El resto tienen valores fijos, que no cambian.
        
        // Añadimos el marcador con ventanita para el Arduino (los valores de temperatura y humedad 
        // serán modificados la primera vez que se haga click en el marcador y se consulte al Arduino)
        this.addMarcadorConInfoWindow("Escuela Ingeniería Informatica", this.latitudArduino, this.longitudArduino, 0, 0); 
        // Añadimos el resto de marcadores con "sensores falsos"
        this.addMarcadorConInfoWindow("Facultad de Magisterio", 43.357738, -5.855048, 20, 32);
        this.addMarcadorConInfoWindow("Facultad de Ciencias ", 43.357936, -5.853565, 21, 30);
            
        //this.activarMarcadores(mapa); // TODO - quitar??
        //this.mapa.setCenter(this.posicionActual); // TODO - quitar
        //this.marcadorPosicionActual.setTitle("Tu posición"); // TODO - qutar
    }
    
    /*//Activa todos los marcadores. // TODO - quitar
    activarMarcadores(mapa) {
        for (var i = 0; i < this.marcadores.length; i++) {
            this.marcadores[i].setMap(mapa);
        }
    }*/

    // Consulta al Arduino la temperatura y la humedad utilizando Ajax con JQuery,
    // y si tiene éxito, actualiza la temperatura y humedad en la web
    cargarTemperaturaYHumedad(){
        var urlArduino = "http://"+ this.IPArduino +"/temperaturayhumedad";
        
        $.ajax({
            dataType: "json",
            url: urlArduino,
            method: 'GET',
            success: this.actualizarTemperaturaYHumedad.bind(this),
            error:function(){
                $("#mensajeError").html("¡En este momento no se puede obtener la información de la temperatura y la humedad!"); 
            }
        });
    }
    
    // Actualiza en en el infowindow la temperatura y la humedad
    actualizarTemperaturaYHumedad(datos){
        $("#temperatura").html(datos.temperatura);
        $("#humedad").html(datos.humedad);
    }

    // Crea un marcador con las coordenadas indicadas y le añade un infowindow con la información indicada
    addMarcadorConInfoWindow(lugar, latitud, longitud, temperatura, humedad){
        var marcador = this.addMarcador(latitud, longitud, lugar);
        this.addInfoWindow(marcador, lugar, latitud, longitud, temperatura, humedad);
    }
    
    // Crea un marcador con las coordenadas indicadas, le pone el title indicado en lugar,
    // lo añade al mapa, y lo almacena en el array de marcadores
    addMarcador(latitud, longitud, lugar) {
        var marcador = new google.maps.Marker({
            position: new google.maps.LatLng(latitud, longitud),
            map: this.mapa,
            title: lugar
        });
        this.marcadores.push(marcador);
        
        return marcador;
    }
    
    // Añade un infowindow al marcador indicado, y con la información indicada
    addInfoWindow(marcador, lugar, latitud, longitud, temperatura, humedad){      
        //Creamos una infoWindow con esta estructura html
        var contentString = '<div id="mensajeError"></div>'+
                            '<div>'+
                                '<div>'+
                                    '<h2>'+ lugar +'</h2>'+
                                '</div>'+
                                '<div id="datos">'+
                                    '<p>Latitud: '+ latitud +'</p>'+
                                    '<p>Longitud: '+ longitud +'</p>'+
                                    '<p>Temperatura:<span id="temperatura">'+ temperatura +'</span></p>'+
                                    '<p>Humedad:<span id="humedad">'+ humedad +'</span></p>'+
                                '</div>'+
                            '</div>';

        var infowindow = new google.maps.InfoWindow({
          content: contentString
        });

        //marcador.setTitle(lugar); // TODO - quitar??
        
        // Añadimos un escuchador del evento click para cuando se pulse ese marcador
        marcador.addListener('click', this.clickInfoWindowHandler.bind(this,marcador,infowindow));
    }
    
    // Cuando se hace click en un marcador, se hable su infoWindow correspondiente
    clickInfoWindowHandler(marcador,infowindow){
        infowindow.open(this.mapa, marcador);
    }
    
}

var mapa = new MapaSensores();
google.maps.event.addDomListener(window, 'load', mapa.inicializar.bind(mapa));