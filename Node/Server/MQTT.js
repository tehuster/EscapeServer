const mosca = require('mosca')
const EventEmitter = require('events')
const mqtt = require('mqtt')

class MQTT extends EventEmitter 
{
    constructor()
    {
        super()
        this.settings = {
            type: 'mqtt',
            host: '192.168.1.208',
            port: 1883
        }; 
        this.broker;
        this.publisher;
        
        this.setupBroker()
        this.setupPublisher()
    }

    setupBroker()
    {
        this.broker = new mosca.Server(this.settings)
        this.broker.on('ready', ()=>{
            console.log('Broker is ready!');    
        })
        
        this.broker.on('published', (packet)=>{
            console.log(`Received [${packet.topic.toString()}] : ${packet.payload.toString()}`);    
        })
    }

    setupPublisher()
    {
        this.publisher = mqtt.connect(`mqtt://${this.settings.host}:${this.settings.port}`)
        this.publisher.on('connect', ()=>{
            console.log('MQTT Publisher initialized')        
        })
    }

    sendMessage(deviceName, type, name, value)
    {
        console.log(`Sending request: ${deviceName}/${type}, ${name}/${value}`)        
        this.publisher.publish(`${deviceName}/${type}`, `${name}/${value}`)
    }
}

module.exports = MQTT;