// A lot of duplicate code, this can be cleaner

function AddDevice()
{
    let room = document.getElementById('room').value;
    let name = document.getElementById('name').value;
    let description = document.getElementById('description').value;
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

function AddAction(id)
{
    let action_name = document.getElementById(`action_name_${id}`).value; 
    let action_parameter = document.getElementById(`action_parameter_${id}`).value;
    if(action_parameter == "")
    {
        action_paramter = null;
    }
    let action_public = document.getElementById(`action_public_${id}`).value;

    socket.emit('server', {
        type: 'devices', 
        command: 'addAction', 
        device_id: id,
        action_name: action_name, 
        action_parameter: action_parameter,        
        actionPublic: action_public 
    }); 
    console.log(`Adding Action: ${action_name}`); 
}

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