module.exports = (Sequelize, db) => {
    const Events = db.define('Events', {
        DeviceId: {
            type: Sequelize.INTEGER,
            references: {
                model: 'Devices',
                key: 'id'
            }
        },
        event_name: {
            type: Sequelize.STRING,
            allowNull: false
        }
    })

    Events.associate = function(models) {
        Events.belongsTo(models.Devices)       
    }   

    Events.sync({ force: false }).then(() => { })

    return Events;
};