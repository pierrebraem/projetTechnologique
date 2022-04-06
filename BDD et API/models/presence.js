const mongoose = require('mongoose')

const presenceSchema = new mongoose.Schema({
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
{ collection: 'presence'})

module.exports = mongoose.model("presence", presenceSchema)