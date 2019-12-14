
function AddTrigger()
{
    let name = document.getElementById('name').value;
    let iDevice = document.getElementById('iDevice').value;
    let event = document.getElementById('event').value;
    let oDevice = document.getElementById('oDevice').value;
    let action = document.getElementById('action').value;
    let parameter = document.getElementById('parameter').value;
    let delay_time = document.getElementById('delay_time').value;

    console.log(name, iDevice, event, oDevice, action);

    socket.emit('server', {
        type: 'triggers', 
        command: 'add',          
        name: name, 
        iDevice: iDevice,
        event: event, 
        oDevice: oDevice, 
        action: action, 
        parameter: parameter,
        delay_time: delay_time
    });    
}

function RemoveTrigger(id)
{
    console.log("Removing trigger: " + id);

    socket.emit('server', {
        type: 'triggers', 
        command: 'remove',          
        id: id
    });    
}
