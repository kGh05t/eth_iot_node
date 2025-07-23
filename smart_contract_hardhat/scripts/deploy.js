const hre = require("hardhat");

async function main() {
  const SensorDataStorage = await hre.ethers.getContractFactory("SensorDataStorage");
  const contract = await SensorDataStorage.deploy();

  await contract.waitForDeployment();

  console.log("Contract deployed to:", await contract.getAddress());
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
