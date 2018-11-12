"use strict";
class MapaMeteorologico{
    constructor(){
        this.mapa;
        this.posicionActual;
        
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

        //this.mapa.setCenter(this.posicionActual); // TODO - quitar
        //this.marcadorPosicionActual.setTitle("Tu posición"); // TODO - qutar
    }
    
    addMarcadorConMeteorologicInfoWindow(){
        this.addMeteorologicInfoWindow();
    }
    
    addMarcadorPosicionActual() { // TODO  cambiar
        //Si habia antes un marcador para la posicion actual, lo borramos del mapa
        if(this.marcadorPosicionActual!=null)
            this.marcadorPosicionActual.setMap(null);
        
        //Creamos un icono azul y lo ponemos en la posicion actual del dispositivo
        var image = "iconos/marcador_azul.png";
        this.marcadorPosicionActual = new google.maps.Marker({
            position: this.posicionActual,
            map: this.mapa,
            icon: image
        });
    }
    
    addMeteorologicInfoWindow(nombre, latitud, longitud, temperatura, humedad){
        //Creamos una infoWindow con esta estructura html
        var contentString = '<div id="mensajeError"></div>'+
                            '<div id="seccionTiempo">'+
                                '<div id="cabecera">'+
                                    '<h2>'+ nombre +'</h2>'+
                                '</div>'+
                                '<div id="datos">'+
                                    '<p>Latitud: '+ latitud +'</p>'+
                                    '<p>Longitud: '+ longitud +'</p>'+
                                    '<p>Temperatura: '+ temperatura +'</p>'+
                                    '<p>Humedad: '+ humedad +'</p>'+
                                '</div>'+
                            '</div>';
        
        this.infowindow = new google.maps.InfoWindow({
          content: contentString
        });
        
        //Guardamos los datos meteorologicos de la posicion actual
        var latitud = this.posicionActual.lat();
        var longitud = this.posicionActual.lng();
        this.cargarDatosMeteorologicos(latitud,longitud);
        
        //Le añadimos al marcador de la posicion actual un escuchador al evento click, para que pase a ejecutar la funcion que muestra la informacion meteorologica
        this.marcadorPosicionActual.addListener('click', this.abrirInformacionMeteorologica.bind(this));
    }
    
    abrirInformacionMeteorologica(){
        this.infowindow.open(this.mapa, this.marcadorPosicionActual);//Abrimos la info window
        this.mostrarDatosTiempo();//Mostramos los datos del tiempo que teniamos almacenados previamente
    }
    
    cargarDatosMeteorologicos(latitud,longitud){
        var urlOpenWeather = "http://api.openweathermap.org/data/2.5/weather?lat="+ latitud +"&lon="+ longitud + this.unidades + this.idioma + "&APPID=" + this.apikeyOpenWeather;
        
        $.ajax({
            dataType: "json",
            url: urlOpenWeather,
            method: 'GET',
            success: this.guardarDatosMeteorologicos.bind(this),
            error:function(){
                $("#mensajeError").html("¡En este momento no se puede obtener la información meteorológica!"); 
            }
        });
    }
    
    guardarDatosMeteorologicos(datos){
        this.horaMedida = new Date(datos.dt *1000).toLocaleTimeString();
        this.fechaMedida = new Date(datos.dt *1000).toLocaleDateString();
        this.ultimaMedida = this.fechaMedida + " " + this.horaMedida; 
        this.nubosidad =  datos.clouds.all;
        this.humedad = datos.main.humidity;
        this.presion = datos.main.pressure;
        var codigoIcono = datos.weather[0].icon;
        this.srcIconoTiempo = "http://openweathermap.org/img/w/" + codigoIcono + ".png";
        this.gradosTemp = datos.main.temp;
        this.gradosTempMin = datos.main.temp_min;
        this.gradosTempMax = datos.main.temp_max;
        this.descripcion = datos.weather[0].description;
            this.descripcion = this.descripcion.charAt(0).toUpperCase() + this.descripcion.slice(1);//Pasamos la primera letra de la descripcion a mayúsculas
        this.vientoKM_H = datos.wind.speed * 3.6;
            this.vientoKM_H = this.vientoKM_H.toFixed(2);
        this.direccionViento = datos.wind.deg;
        
        if(this.direccionViento!=null)
            this.direccionViento = this.direccionViento.toFixed(2);
        
        (datos.visibility!=null ? this.visibilidad_KM = datos.visibility / 1000 : this.visibilidad_KM = null );
        this.horaAmanece = new Date(datos.sys.sunrise *1000).toLocaleTimeString();
        this.horaOscurece = new Date(datos.sys.sunset *1000).toLocaleTimeString();

        this.nombreEstacion = datos.name;
        this.latitudEstacion = datos.coord.lat;
        this.longitudEstacion = datos.coord.lon;
        
        
        //Al guardar los datos, abrimos la infoWindow directamente, excepto la primera vez que se llama al metodo,
        //que es cuando se guarda la informacion de la ubicacion actual del dispositivo
        //Dejamos que sea el usuario el que haga click si quiere en la posicion actual para ver la informacion meteorologica
        if(this.buscadoAlgunSitio)
            this.abrirInformacionMeteorologica();
    }
    
    mostrarDatosTiempo(){
        //Si la temperatura es <= de 10ºC, consideramos que esta frio
        var srcIconoTemp;
        if(this.gradosTemp <= 10)
            srcIconoTemp="iconos/frio.png";
        else srcIconoTemp="iconos/calor.png";

        $("#tiempoEn").html("El tiempo en " + this.nombreEstacion);

        $("#ultimaMedida").html("Ultima medición: "+this.ultimaMedida);
        $("#divIconoTiempo").html("<img id='iconoTiempo' src='" + this.srcIconoTiempo + "'/>");
        $("#gradosTemperatura").html(this.gradosTemp + "ºC");
        $("#descripcion").html(this.descripcion);
        $("#divIconoTemperatura").html("<img id='iconoTemperatura' src='" + srcIconoTemp + "'/>");
        $("#gradosMin").html(this.gradosTempMin + "ºC");
        $("#gradosMax").html(this.gradosTempMax + "ºC");    

        var stringRestoDatos = "<ul><li>Nubosidad: " + this.nubosidad + " %</li>";        
                stringRestoDatos += "<li>Humedad: " + this.humedad + " %</li>";        
                stringRestoDatos += "<li>Velocidad viento: " + this.vientoKM_H + " km/h</li>";

                if(this.direccionViento==null)
                    stringRestoDatos += "<li>Dirección viento: No disponible</li>";
                else
                    stringRestoDatos += "<li>Dirección viento: " + this.direccionViento + "º</li>";

                stringRestoDatos += "<li>Presión: " + this.presion + " mb</li>";
                stringRestoDatos += "<li>Amanece a las: " + this.horaAmanece + "</li>";
                stringRestoDatos += "<li>Oscurece a las: " + this.horaOscurece + "</li>";
        
                if(this.visibilidad_KM==null)
                    stringRestoDatos += "<li>Visibilidad: No disponible</li>";
                else
                    stringRestoDatos += "<li>Visibilidad: " + this.visibilidad_KM + " km</li></ul>";
                

        $("#restoDatos").html(stringRestoDatos);

        var coordenadasEstacion = "<ul id='latitudLongitud'><li>Latitud: " + this.latitudEstacion + "º</li>";
            coordenadasEstacion += "<li>Longitud: " + this.longitudEstacion + "º</li></ul>";

        $("#nombreEstacion").html("Estación meteorológica: " + this.nombreEstacion);
        $("#coordenadasEstacion").html(coordenadasEstacion);
    }


    
    
    
    
    
    
    
    addMarcadorConInfoWindow(localizacion,nombre,puntuacion,direccion){
        var marcador = this.addMarcador(localizacion,nombre);
        this.addInfoWindow(marcador,nombre,puntuacion,direccion);
    }
    
    //Añade un marcador y lo almacena en el array
    addMarcador(localizacion,nombre) {
        var marcador = new google.maps.Marker({
            position: localizacion,
            map: this.mapa,
            title: nombre
        });
        this.marcadores.push(marcador);
        return marcador;
    }
    
    addInfoWindow(marcador,nombre,puntuacion,direccion){
        var contentString = '<div id="content">'+
                                '<h1>'+ nombre +'</h1>'+
                                '<div id="bodyContent">'+
                                    '<ul>'+
                                        '<li> Dirección: '+ direccion +'</li>'+
                                        '<li> Puntuación: '+ puntuacion +'</li>'+
                                    '</ul>'+
                                '</div>'+
                            '</div>';

        var infowindow = new google.maps.InfoWindow({
          content: contentString
        });

        marcador.setTitle(nombre);
        
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