module.exports = (Sequelize, db) => {
    const Triggers = db.define('Triggers', {
        trigger_name: {
            type: Sequelize.STRING,
        },
        input_device: {
            type: Sequelize.INTEGER,            
        },
        input_action: {
            type: Sequelize.INTEGER,           
        },       
        output_device: {
            type: Sequelize.INTEGER,            
        },
        output_action: {
            type: Sequelize.INTEGER,  
        },
        delay_time: {
            type: Sequelize.INTEGER,
            allowNull: true
        }
    })

    Triggers.associate = function(models) {
       
    }

    Triggers.sync({ force: false }).then(() => { })

    return Triggers;
}