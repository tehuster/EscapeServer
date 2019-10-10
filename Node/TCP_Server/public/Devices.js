
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
        address: address, 
        room: room,
        name: name, 
        description: description,
        config: config, 
        actions: actions,
    });
}