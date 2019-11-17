module.exports = (Sequelize, db) => {
    const Triggers = db.define('Triggers', {
        trigger_name: {
            type: Sequelize.STRING,
        },
        iDevice: {
            type: Sequelize.INTEGER, 
            references: {
                model: 'Devices',
                key: 'id'
            }           
        },
        iAction: {
            type: Sequelize.STRING,           
        },       
        oDevice: {
            type: Sequelize.INTEGER,   
            references: {
                model: 'Devices',
                key: 'id'
            }         
        },
        oAction: {
            type: Sequelize.STRING,  
        },
        delay_time: {
            type: Sequelize.INTEGER,
            allowNull: true
        }
    })

    Triggers.associate = function(models) {
    //    Triggers.hasOne(models.Devices, { foreignKey: 'id', as: 'input_device' })
    //    Triggers.hasOne(models.Devices, { foreignKey: 'id', as: 'output_device' })
    }

    Triggers.sync({ force: false }).then(() => { })

    return Triggers;
}