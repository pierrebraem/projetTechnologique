const mongoose = require('mongoose')

const lumiereSchema = new mongoose.Schema({
    date:{
        type: Date,
        required: true
    },
    heure:{
        type: String,
        required: true
    },
    evenement:{
        type: String,
        required: true
    },
    peripherique:{
        type: Object,
        required: true
    }
},
{ collection: 'lumiere'})

module.exports = mongoose.model("lumiere", lumiereSchema)