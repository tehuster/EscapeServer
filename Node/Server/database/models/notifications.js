module.exports = (Sequelize, db) => {
    const Notifications = db.define('Notifications', {
        topic_name: {
            type: Sequelize.STRING,
        },
        topic_type: {
            type: Sequelize.STRING,
        },
        payload_0: {
            type: Sequelize.STRING,
        },
        payload_1: {
            type: Sequelize.STRING,
        },
        notification_color: {
            type: Sequelize.STRING,
        }
    })

    Notifications.associate = function(models) {       
    }   

    Notifications.sync({ force: false }).then(() => { })

    return Notifications;
};