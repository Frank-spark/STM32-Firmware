#include "EthernetSetup.h"

// Ethernet Handle
ETH_HandleTypeDef heth;

EthernetSetup::EthernetSetup() {}

bool EthernetSetup::initialize() {
    configurePins();

    __HAL_RCC_ETH_CLK_ENABLE(); // Enable Ethernet clock

    // Initialize Ethernet MAC and PHY
    heth.Instance = ETH;
    heth.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    heth.Init.Speed = ETH_SPEED_100M;
    heth.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    heth.Init.PhyAddress = LAN8742A_PHY_ADDRESS;

    if (HAL_ETH_Init(&heth) != HAL_OK) {
        return false; // Initialization failed
    }

    return true; // Ethernet initialized successfully
}

void EthernetSetup::configurePins() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIO clocks for Ethernet pins
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // Configure Ethernet RMII pins
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}
