
function AddTrigger()
{
    let name = document.getElementById('name').value;
    let iDevice = document.getElementById('iDevice').value;
    let iAction = document.getElementById('iAction').value;
    let oDevice = document.getElementById('oDevice').value;
    let oAction = document.getElementById('oAction').value;

    console.log(name, iDevice, iAction, oDevice, oAction);

    socket.emit('server', {
        type: 'triggers', 
        command: 'add',          
        name: name, 
        iDevice: iDevice,
        iAction: iAction, 
        oDevice: oDevice, 
        oAction: oAction
    });    
}