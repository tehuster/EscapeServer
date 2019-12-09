module.exports = (Sequelize, db) => {
    const Parameters = db.define('Parameters', {
        ActionId: {
            type: Sequelize.INTEGER,
            references: {
                model: 'Actions',
                key: 'id'
            }
        },
        Parameter_name: {
            type: Sequelize.STRING
        },
        Parameter_type: {
            type: Sequelize.STRING
        },
        Parameter_value: {
            type: Sequelize.STRING
        },
        public: {
            type: Sequelize.BOOLEAN
        }
    })

    Parameters.associate = function(models) {
        Parameters.belongsTo(models.Actions)       
    }   

    Parameters.sync({ force: false }).then(() => { })

    return Parameters;
};