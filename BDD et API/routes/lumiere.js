const express = require('express')
const { route } = require('express/lib/application')
const router = express.Router()
const lumieremodel = require('../models/lumiere')

//Ajouter un nouvelle historique
router.post('/', async (req, res) => {
    const lumiere = new lumieremodel({
        date: req.body.date,
        heure: req.body.heure,
        evenement: req.body.evenement,
        peripherique: req.body.peripherique
    })

    try{
        const nouvellelumiere = await lumiere.save()
        res.status(201).json(nouvellelumiere)
    } catch(err){
        res.status(400).json({message: err.message})
    }
})

//Affiche toute l'historique
router.get('/', async (req, res) => {
    try{
        const lumiere = await lumieremodel.find()
        res.json(lumiere)
    } catch(err){
        res.status(500).json({message: err.message})
    }
})

//Affiche l'historique d'une lumière
router.get('/:nom', getHistorique, async(req, res) =>{
    res.send(res.historique)
})

async function getHistorique(req, res, next){
    let historique
    try{
        //console.log(donnees)
        historique = await lumieremodel.find({"peripherique.nom": req.params.nom})
        //console.log(historique)
        if(historique == null){
            return res.status(404).json({ message: "périphérique inconnu" })
        }
    }catch(err){
        return res.status(500).json({ message: err.message })
    }
    res.historique = historique
    next()
}
module.exports = router