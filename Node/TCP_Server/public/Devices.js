
function AddDevice()
{
    let address = document.getElementById('address').value;
    let room = document.getElementById('room').value;
    let name = document.getElementById('name').value;
    let description = document.getElementById('description').value;
    let config = document.getElementById('config').value;
    let actions = document.getElementById('actions').value;
    
    console.log(`Adding device: ${name}`);    
    socket.emit('server', {
        type: 'devices', 
        command: 'add',         
        room: room,
        address: address, 
        name: name, 
        description: description,
        config: config, 
        actions: actions,
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