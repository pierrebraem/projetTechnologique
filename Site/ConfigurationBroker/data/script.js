//Fonction qui récupère les textes "adresse IP" et "port" et l'envoie à MyServer.cpp
function ConfigBroker(){
    var adresseIP = "adresseip=" + document.getElementById("adresseIP").value;
    var port = "port=" + document.getElementById("port").value;
    var params = adresseIP + "&" + port;

    xhttp = new XMLHttpRequest();
    xhttp.open("POST", "ConfigBroker", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xhttp.send(params);

    xhttp.onreadystatechange = function(){
        if(this.readyState == 4 && this.status == 200){
            alert(this.responseText);
        }
    }
}

//Fonction qui permet la validation de la connexion (retourne true si les informations sont vrais sinon false)
function Connexion(){
    var user = "user=" + document.getElementById("user").value;
    var mdp = "mdp=" + document.getElementById("mdp").value; 
    var params = user + "&" + mdp;

    xhttp = new XMLHttpRequest();
    xhttp.open("POST", "connexion", true);
    xhttp.send(params);

    xhttp.onreadystatechange = function(){
        if(this.readyState == 4 && this.status == 200){
            var response = this.responseText;
            if(response == "true"){
                window.location.href = "/configuration";
            }
            else{
                alert("Nom d'utilisateur ou mot de passe incorrect");
            }
        }
    }
}