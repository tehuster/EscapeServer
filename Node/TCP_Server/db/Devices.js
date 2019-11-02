const Sequelize = require('sequelize')
const db = require('./DB')

const Devices = db.define('Devices', {
  device_room: {
    type: Sequelize.STRING,
    allowNull: false
  },
  device_name: {
    type: Sequelize.STRING,
    allowNull: false
  },
  device_description: {
    type: Sequelize.STRING,
    allowNull: true
  }
});

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
  }
});

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
  }
});

module.exports = { Devices, Actions, Configs };

