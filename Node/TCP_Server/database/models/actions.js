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
        public: {
            type: Sequelize.BOOLEAN,
            allowNull: false
        }
    })

    Actions.associate = function(models) {
        Actions.belongsTo(models.Devices)       
    }   

    Actions.sync({ force: false }).then(() => { })

    return Actions;
};