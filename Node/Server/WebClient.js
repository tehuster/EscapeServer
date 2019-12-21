class WebClient {
   constructor(devices, hints, triggers, notifications) {
      const express = require("express")
      const path = require('path')
      const bodyParser = require('body-parser')
      const app = express()
      const os = require('os')
      const networkInterfaces = os.networkInterfaces()

      app.locals.moment = require('moment');
      app.use(bodyParser.json())
      app.use(express.static(path.join(__dirname, '/node_modules/bulma/css')))
      app.use(express.static(path.join(__dirname, '/public')))
      app.use(express.static(path.join(__dirname, '/view')))
      app.set('view engine', 'pug')

      this.ip = networkInterfaces.Ethernet[1].address;

      this.server = require('http').Server(app)
      this.initRoutes(app)
      this.initServer()
      this.devices = devices;
      this.triggers = triggers;
      this.notifications = notifications;
      this.hints = hints;
   }

   initRoutes(app) {
      app.get('/', (req, res) => {
         this.getAllData()
         .then((data) =>{            
            res.render('dashboard', {
               devices: data[0],
               triggers: data[1],
               notifications: data[2],
               server_ip: this.ip
            })
         })
      })

      app.get('/dashboard', (req, res) => {
         this.getAllData()
         .then((data) =>{            
            res.render('dashboard', {
               devices: data[0],
               triggers: data[1],
               notifications: data[2],
               server_ip: this.ip
            })
         })
      })

      app.get('/hints', (req, res) => {
         this.hints.get()
            .then(data => res.render('hints', { hints: data }))
      })

      app.get('/devices', (req, res) => {
         this.getAllData()
         .then((data) =>{            
            res.render('devices', {
               devices: data[0],
               triggers: data[1],
               notifications: data[2],
               server_ip: this.ip
            })
         })
      })

      app.get('/triggers', (req, res) => {
         this.getAllData()
         .then((data) =>{            
            res.render('triggers', {
               devices: data[0],
               triggers: data[1],
               notifications: data[2],
               server_ip: this.ip
            })
         })
      })

      app.get('/api', (req, res) => {
         let devices;
         let triggers;
         this.devices.findAll({
            include: ['actions', 'configs', 'events']
         })
            .then((device_data) => {
               devices = device_data;
               this.triggers.findAll()
                  .then((trigger_data) => {
                     triggers = trigger_data;
                     res.json({
                        devices: devices,
                        triggers: triggers
                     })
                  })
            })
      })

      app.get('/layout', (req, res) => {
         this.getAllData()
         .then((data) =>{            
            res.render('layout', {
               devices: data[0],
               triggers: data[1],
               notifications: data[2],
               server_ip: this.ip
            })
         })
      })

      app.get('/old', (req, res) => {
         this.getAllData()
         .then((data) =>{            
            res.render('index', {
               devices: data[0],
               triggers: data[1],
               notifications: data[2],
               server_ip: this.ip
            })
         })
      })
   }

   async getAllData() {
      return await Promise.all(
         [
            this.devices.findAll({
               include: ['actions', 'configs', 'events']
            }),
            this.triggers.findAll(),
            this.notifications.findAll({
               order: [
                  ['createdAt', 'DESC']
               ],
               limit: 100
            })
         ]
      )
   }      

   initServer() {
      this.server.listen(3000, this.ip, 0, () => {
         console.log(`Webserver is listening on ${this.ip}`)
      });
   }
}

module.exports = WebClient;



