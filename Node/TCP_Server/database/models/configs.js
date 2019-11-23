module.exports = (Sequelize, db) => {
    const Configs = db.define('Configs', {
        DeviceId: {
            type: Sequelize.INTEGER,
            references: {
                model: 'Devices',
                key: 'id'
            }
        },
        config_name: {
            type: Sequelize.STRING,
            allowNull: false
        },
        config_type: {
            type: Sequelize.STRING,
            allowNull: true
        },
        public: {
            type: Sequelize.BOOLEAN,
            allowNull: false
        }
    })

    Configs.associate = function(models) {
        Configs.belongsTo(models.Devices)
    }    

    Configs.sync({ force: false }).then(() => { })

    return Configs;
}