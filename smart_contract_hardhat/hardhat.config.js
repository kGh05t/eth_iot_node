require("@nomicfoundation/hardhat-toolbox");

module.exports = {
    solidity: "0.8.20",
    networks: {
        ganache: {
            url: "http://127.0.0.1:7545",
            accounts: ["0x4db6ddec51bd4e35a718d2a7b8cd8c9cb78ac37cac1f8b3342378e6bcbdab784"]  // Replace with your Ganache account private key
        }
    }
};
