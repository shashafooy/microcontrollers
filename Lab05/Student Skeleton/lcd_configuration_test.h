#ifndef LCD_CONFIGURATION_TEST_H
#define LCD_CONFIGURATION_TEST_H



//Function that tests the configuration of the SPI unit and connections to the LCD. 
//
//This function assumes that the GPIO Pin and SSI module have been configured and enabled. 
//
//cmd_data_select_gpio_pin_number is the pin number (0-7) of the gpio pin being used as a command/data select 
//cmd_data_select_gpio_base_address is the base address for the for the gpio port being used as a command/data select
//spi_base_address is the base address for the SPI/SSI peripheral being used. 
void test_configuration(unsigned int cmd_data_select_gpio_pin_number, unsigned char * cmd_data_select_gpio_base_address, unsigned int* spi_base_address);

//Function that configures the LCD screen after the SPI and GPIO have been set up. 
//cmd_data_select_gpio_pin_number is the pin number (0-7) of the gpio pin being used as a command/data select 
//cmd_data_select_gpio_base_address is the base address for the for the gpio port being used as a command/data select
//spi_base_address is the base address for the SPI/SSI peripheral being used. 
void configure_lcd_commands(unsigned int cmd_data_select_gpio_pin_number, unsigned char * cmd_data_select_gpio_base_address, unsigned int* spi_base_address);

//Function that writes data to the lcd screen
//data is the byte of data being written
void lcd_configuration_write_data(unsigned char data);

//Function that writes data to the lcd screen
//data is the two bytes of data being written
//this function is useful for writing colors, which are sent in two-byte packets
void lcd_configuration_write_data_two_bytes(unsigned short data);

//Function that writes a command to the lcd screen
//command is the command byte to be written.
void lcd_configuration_write_command(unsigned char command);

//Function that flashes the entire screen one color
//
//Command assumes that the writing data commands are already set up.
void lcd_configuration_flash_screen(unsigned short color);




#endif
