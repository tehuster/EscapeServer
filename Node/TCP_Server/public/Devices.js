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
    socket.emit('server', {
        type: 'devices', 
        command: 'addAction', 
        device_id: id,
        action_name: action_name, 
        action_parameter: action_parameter
    }); 
    console.log(`Adding Action: ${action_name}`); 
}

function AddConfig(id)
{
    let config_name = document.getElementById(`config_name_${id}`).value;
    socket.emit('server', {
        type: 'devices', 
        command: 'addConfig', 
        device_id: id,
        config_name: config_name, 
    }); 
    console.log(`Adding Config: ${config_name}`); 
}

function RequestAction(action_id)
{
    console.log(action_id)
    socket.emit('server', {
        type: 'request', 
        command: 'action', 
        action_id: action_id
    });     
}

function GETConfig(config_id)
{
    console.log(config_id)
    socket.emit('server', {
        type: 'request', 
        command: 'config_get', 
        config_id: config_id
    });     
}

function SETConfig(config_id)
{
    console.log(config_id)
    socket.emit('server', {
        type: 'request', 
        command: 'config_set', 
        config_id: config_id
    });     
}