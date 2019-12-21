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
            host: '192.168.1.94', //'192.168.1.94' NUC @ Home   '192.168.1.208' PC @ Home
            port: 1883
        }; 
        this.broker;
        this.publisher;
        
        this.setupBroker()
        this.setupPublisher()

        this.triggers;
    }

    setupBroker()
    {
        this.broker = new mosca.Server(this.settings)
        this.broker.on('ready', ()=>{
            console.log('Broker is ready!');    
        })
        
        this.broker.on('published', (packet)=>{
            console.log(`Received [${packet.topic.toString()}] : ${packet.payload.toString()}`); 
            let topic = packet.topic.toString().split('/')
            let payload = packet.payload.toString().split('/')
                       
            let device_name = topic[0];
            let topic_type = topic[1];
            let payload_0 = payload[0];
            let payload_1 = payload[1];

            // console.log(`${device_name}  ${topic_type}  ${payload_0}  ${payload_1}`);
            
            switch (topic_type) {
                case 'Event':
                    this.EventHandler(device_name, payload_0, payload_1)
                    break;
                case 'Error':
                    break;
                case 'Response':
                    break;
                case 'Action':
                    break;
                default:
                   console.log(`Unknown topic type: ${topic_type}`)                   
            }

            this.emit('notification', {
                device_name, 
                topic_type, 
                payload_0, 
                payload_1
            })
        })
    }

    EventHandler(name, p0, p1)
    {          
        this.triggers.findAll({
        })
        .then((data) => {
            data.forEach(trigger => {            
                if(trigger.iDevice == name && trigger.event == p0)
                {
                    setTimeout(() => {
                        this.sendMessage(trigger.oDevice, 'Action', trigger.action, trigger.parameter)
                    }, trigger.delay_time * 1000);                
                }
            });      
        });     
          
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
        let settings = {           
            qos: 2, // 0, 1, or 2
            retain: false // or true
        };
          
        this.publisher.publish(`${deviceName}/${type}`, `${name}/${value}`, settings, () => {

            console.log(`Message send: ${deviceName}/${type}, ${name}/${value}`)   
        });
    }
}

module.exports = MQTT;