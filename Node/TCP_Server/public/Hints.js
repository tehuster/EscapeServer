
function AddHint()
{
    let room = document.getElementById('room').value;
    let hint = document.getElementById('hint').value;
    console.log(`Adding hint: ${hint} @ ${room}`);    
    socket.emit('server', {type: 'hints', command: 'add', room: room, hint: hint});
}

