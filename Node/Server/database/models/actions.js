module.exports = (Sequelize, db) => {
    const Actions = db.define('Actions', {
        DeviceId: {
            type: Sequelize.INTEGER,
            references: {
                model: 'Devices',
                key: 'id'
            }
        },
        action_name: {
            type: Sequelize.STRING,
            allowNull: false
        },
        action_parameter_type: {
            type: Sequelize.STRING,
            allowNull: true
        },
        action_public: {
            type: Sequelize.STRING,
            allowNull: false
        }
    })

    Actions.associate = function(models) {
        Actions.belongsTo(models.Devices)  
        Actions.hasMany(models.Parameters, { as: 'parameters' })     
    }   

    Actions.sync({ force: false }).then(() => { })

    return Actions;
};