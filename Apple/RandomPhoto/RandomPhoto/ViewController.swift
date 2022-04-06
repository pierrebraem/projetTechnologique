//
//  ViewController.swift
//  RandomPhoto
//
//  Created by Usager on 22-03-29.
//  Copyright © 2022 Usager. All rights reserved.
//
// Le tuto est à cette URL : https://www.youtube.com/watch?v=yuo50-TiKgo

import UIKit

class ViewController: UIViewController {

    //Initialise les propriétés d'une image
    private let imageView: UIImageView = {
        //Initialise un objet image (ici "imageView")
        let imageView = UIImageView()
        //L'image prendra toute la zone qui lui est dédier
        imageView.contentMode = .scaleAspectFill
        //La couleur de la zone de l'image sera blanche
        imageView.backgroundColor = .white
        return imageView
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        //Change la couleur de font en rouge
        view.backgroundColor = .red
        //Ajoute l'image dans la vue
        view.addSubview(imageView)
        //Défini la position et la taille de l'image (unité en pixel)
        imageView.frame = CGRect(x: 0, y: 0, width: 300, height: 300)
        //Position l'image au centre du template
        imageView.center = view.center
        //Appel la fonction getRandomPhoto
        getRandomPhoto()
    }
    
    //Fonction qui va permettre de charger une photo aléatoirement à partir d'un URL
    func getRandomPhoto(){
        let urlString = "https://source.unsplash.com/random/600x600"
        //Précise au code que c'est un URL" La variable devient un type URL
        let url = URL(string: urlString)!
        //Récupère le contenu de l'URL. Si il échoue, alors il retournera du vide
        guard let data = try? Data(contentsOf: url) else {
            return
        }
        //Va générer l'image à partir des propriétés déclarer si haut
        imageView.image = UIImage(data: data)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

