// A lot of duplicate code, this can be cleaner

////////////////////////////////////// DEVICES
function AddDevice()
{
    let room = document.getElementById('device_room').value;
    let name = document.getElementById('device_name').value;
    let description = document.getElementById('device_description').value;
    // let config = document.getElementById('config').value;
    // let actions = document.getElementById('actions').value;
    
    console.log(`Adding device: ${name}`);    
    socket.emit('server', {
        type: 'devices', 
        command: 'add',         
        room: room,       
        name: name, 
        description: description,
        // config: config, 
        // actions: actions,
    });
}

function RemoveDevice(id)
{    
    console.log(`Removing device: ${id}`);    
    socket.emit('server', {
        type: 'devices', 
        command: 'remove', 
        id: id, 
    });
}

////////////////////////////////////// ACTIONS
function AddAction()
{
    let device_id = document.getElementById(`device_id`).value; 
    let action_name = document.getElementById(`action_name`).value; 
    let action_parameter = document.getElementById(`action_parameter`).value;
    let action_public = document.getElementById(`action_public`).value;   

    socket.emit('server', {
        type: 'devices', 
        command: 'addAction', 
        device_id: device_id,
        action_name: action_name, 
        action_parameter: action_parameter,        
        action_public: action_public 
    }); 
    console.log(`Adding Action: ${action_name}`); 
}

function RemoveAction(id)
{
    console.log(`Removing action: ${id}`);    
    socket.emit('server', {
        type: 'devices', 
        command: 'removeAction', 
        id: id, 
    });
}

function RequestAction(device_name, action_name, id)  //Add paramaters
{
    let parameter = null;
    if(document.getElementById(`parameter_${id}`) != undefined)
    {
        parameter = document.getElementById(`parameter_${id}`).value; 
    }   
    
    console.log(`Requesting: ${action_name} @ ${device_name} : ${parameter}`)
    socket.emit('server', {
        type: 'request', 
        command: 'action', 
        deviceName: device_name,
        actionName: action_name,
        actionParameter: parameter
    });     
}


////////////////////////////////////// EVENTS
function AddEvent(id)
{
    let device_id = document.getElementById(`device_id_event`).value; 
    let event_name = document.getElementById(`event_name`).value;
    socket.emit('server', {
        type: 'devices', 
        command: 'addEvent', 
        device_id: device_id,
        event_name: event_name
    }); 
    console.log(`Adding Event: ${event_name}`); 
}

function RemoveEvent(id)
{
    console.log(`Removing event: ${id}`);    
    socket.emit('server', {
        type: 'devices', 
        command: 'removeEvent', 
        id: id, 
    });
}

function SimulateEvent(device_name, event_name)
{
    console.log(`Simulate event: ${event_name}`);    
    socket.emit('server', {
        type: 'request', 
        command: 'event', 
        deviceName: device_name,
        actionName: event_name
    });    
}

////////////////////////////////////// CONFIGS
function AddConfig(id)
{
    let config_name = document.getElementById(`config_name_${id}`).value;
    let config_public = document.getElementById(`config_public_${id}`).value;
    socket.emit('server', {
        type: 'devices', 
        command: 'addConfig', 
        device_id: id,
        config_name: config_name,
        config_public: config_public 
    }); 
    console.log(`Adding Config: ${config_name}`); 
}

function GETConfig(config_name)
{
    console.log(config_name)
    // socket.emit('server', {
    //     type: 'request', 
    //     command: 'config_get', 
    //     config_name: config_name
    // });     
}

function SETConfig(config_name)
{
    console.log(config_name)
    // socket.emit('server', {
    //     type: 'request', 
    //     command: 'config_set', 
    //     config_name: config_name
    // });     
}