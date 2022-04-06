require('dotenv').config()

const express = require('express')
const app = express()
const mongoose = require('mongoose')

mongoose.connect(process.env.DATABASE_URL, { useNewUrlParser: true})
const tdb = mongoose.connection
tdb.on('error', (error) => console.error(error))
tdb.once('open', () => console.log("Connecté à la base de données"))

app.use(express.json())

const presenceRouter = require('./routes/presence')

app.use('/api/presence', presenceRouter)

app.listen(3000, () => console.log("L'API a démarré"))