"use strict";
class MapaMeteorologico{
    constructor(){
        this.mapa;
        this.marcadores=[]; //Array para guardar los marcadores
        
        this.apikeyOpenWeather = "7fc07660c3a999e7e1a9e6f53a925d92"; // TODO - quitar
        this.unidades = "&units=metric"; // TODO - quitar
        this.idioma = "&lang=es"; // TODO - quitar
        
        this.temperatura;
        this.humedad;
                
        this.infowindow;
    }
    
    inicializar() {
        var estiloNocturno = this.getEstiloNocturno();//Creamos un estilo nocturno
        
        var mapaOpciones = {
            zoom: 15,
            mapTypeControlOptions: {
                mapTypeIds: ['roadmap', 'satellite', 'hybrid', 'terrain', 'nocturno']
            },
            mapTypeId: google.maps.MapTypeId.ROADMAP
        };
        
        this.mapa = new google.maps.Map(document.getElementById('mapa-canvas'), mapaOpciones);
        this.mapa.mapTypes.set('nocturno', estiloNocturno);//Asociamos el 'typeId' nocturno con el estilo 'estiloNocturno'
        this.mapa.setMapTypeId('nocturno');//Por defecto, le ponemos el modo nocturno

        this.addMarcadorConInfoWindow("Escuela informatica", 1111.11, 1111.11, 0, 0); // TODO - cambiar
        this.addMarcadorConInfoWindow("Escuela informatica 2", 1112.11, 1112.11, 10, 10); // TODO - cambiar
            
        this.activarMarcadores(mapa);
        //this.mapa.setCenter(this.posicionActual); // TODO - quitar
        //this.marcadorPosicionActual.setTitle("Tu posición"); // TODO - qutar
    }
    
    //Activa todos los marcadores.
    activarMarcadores(mapa) {
        for (var i = 0; i < this.marcadores.length; i++) {
            this.marcadores[i].setMap(mapa);
        }
    }

        
    
    cargarDatosMeteorologicos(latitud,longitud){
        var urlArduino = "http://192.168.61.37/temperaturayhumedad";
        
        $.ajax({
            dataType: "json",
            url: urlArduino,
            method: 'GET',
            success: this.guardarTemperaturaYHumedad.bind(this),
            error:function(){
                $("#mensajeError").html("¡En este momento no se puede obtener la información de la temperatura y la humedad!"); 
            }
        });
    }
    
    guardarTemperaturaYHumedad(datos){
        this.temperatura = datos.temperatura; // TODO - revisar
        this.humedad = datos.humedad; // TODO - revisar
    }
    
    mostrarDatosTiempo(){
        $("#temperatura").html(this.temperatura);
        $("#humedad").html(this.humedad);
    }


    
    
    
    
    
    
    
    
    
    
    addMarcadorConInfoWindow(lugar, latitud, longitud, temperatura, humedad){
        var marcador = this.addMarcador(latitud, longitud, lugar);
        this.addInfoWindow(marcador, lugar, latitud, longitud, temperatura, humedad);
    }
    
    //Añade un marcador y lo almacena en el array
    addMarcador(latitud, longitud, lugar) {
        var marcador = new google.maps.Marker({
            position: new google.maps.LatLng(latitud, longitud); //(-33.8665433,151.1956316);,
            map: this.mapa,
            title: lugar
        });
        this.marcadores.push(marcador);
        
        return marcador;
    }
    
    addInfoWindow(marcador, lugar, latitud, longitud, temperatura, humedad){      
        //Creamos una infoWindow con esta estructura html
        var contentString = '<div id="mensajeError"></div>'+
                            '<div id="content">'+
                                '<div id="cabecera">'+
                                    '<h2>'+ lugar +'</h2>'+
                                '</div>'+
                                '<div id="datos">'+
                                    '<p>Latitud: '+ latitud +'</p>'+
                                    '<p>Longitud: '+ longitud +'</p>'+
                                    '<p>Temperatura: '+ temperatura +'</p>'+
                                    '<p>Humedad: '+ humedad +'</p>'+
                                '</div>'+
                            '</div>';

        var infowindow = new google.maps.InfoWindow({
          content: contentString
        });

        marcador.setTitle(lugar);
        
        marcador.addListener('click', this.clickInfoWindowHandler.bind(this,marcador,infowindow));
    }
    
    clickInfoWindowHandler(marcador,infowindow){
        infowindow.open(this.mapa, marcador);
    }
    
    getEstiloNocturno(){
        return new google.maps.StyledMapType(
            [
            {elementType: 'geometry', stylers: [{color: '#242f3e'}]},
            {elementType: 'labels.text.stroke', stylers: [{color: '#242f3e'}]},
            {elementType: 'labels.text.fill', stylers: [{color: '#746855'}]},
            {
              featureType: 'administrative.locality',
              elementType: 'labels.text.fill',
              stylers: [{color: '#d59563'}]
            },
            {
              featureType: 'poi',
              elementType: 'labels.text.fill',
              stylers: [{color: '#d59563'}]
            },
            {
              featureType: 'poi.park',
              elementType: 'geometry',
              stylers: [{color: '#263c3f'}]
            },
            {
              featureType: 'poi.park',
              elementType: 'labels.text.fill',
              stylers: [{color: '#6b9a76'}]
            },
            {
              featureType: 'road',
              elementType: 'geometry',
              stylers: [{color: '#38414e'}]
            },
            {
              featureType: 'road',
              elementType: 'geometry.stroke',
              stylers: [{color: '#212a37'}]
            },
            {
              featureType: 'road',
              elementType: 'labels.text.fill',
              stylers: [{color: '#9ca5b3'}]
            },
            {
              featureType: 'road.highway',
              elementType: 'geometry',
              stylers: [{color: '#746855'}]
            },
            {
              featureType: 'road.highway',
              elementType: 'geometry.stroke',
              stylers: [{color: '#1f2835'}]
            },
            {
              featureType: 'road.highway',
              elementType: 'labels.text.fill',
              stylers: [{color: '#f3d19c'}]
            },
            {
              featureType: 'transit',
              elementType: 'geometry',
              stylers: [{color: '#2f3948'}]
            },
            {
              featureType: 'transit.station',
              elementType: 'labels.text.fill',
              stylers: [{color: '#d59563'}]
            },
            {
              featureType: 'water',
              elementType: 'geometry',
              stylers: [{color: '#17263c'}]
            },
            {
              featureType: 'water',
              elementType: 'labels.text.fill',
              stylers: [{color: '#515c6d'}]
            },
            {
              featureType: 'water',
              elementType: 'labels.text.stroke',
              stylers: [{color: '#17263c'}]
            }
          ],
            {name: 'Nocturno'});
    }
    
}

var mapa = new MapaMeteorologico();
google.maps.event.addDomListener(window, 'load', mapa.inicializar.bind(mapa));