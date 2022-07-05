#include <common.h>
#include <command.h>
#include <asm/io.h>

#define IOMUXC_SW_MUX_CTL_PAD_SD1_DATA0 0x020E0340
#define IOMUXC_SW_MUX_CTL_PAD_SD1_DATA1	0x020E033C
#define IOMUXC_SW_MUX_CTL_PAD_SD1_CMD	0x020E0348
#define IOMUXC_SW_MUX_CTL_PAD_SD1_DATA2 0x020E034C
#define GPIO1_DR						0x0209C000
#define GPIO1_GDIR						0x0209C004
#define GPIO1_PSR						0x0209C008

void led_init(void) {
	unsigned long temp;
	temp = 0x05;
	writel(temp, IOMUXC_SW_MUX_CTL_PAD_SD1_DATA0);
	writel(temp, IOMUXC_SW_MUX_CTL_PAD_SD1_DATA1);
	writel(temp, IOMUXC_SW_MUX_CTL_PAD_SD1_CMD);
	writel(temp, IOMUXC_SW_MUX_CTL_PAD_SD1_DATA2);

	temp = 0x0f << 16;
	temp = readl(GPIO1_GDIR) | temp;
	writel(temp, GPIO1_DR);

	temp = ~(0x0f << 16);
	temp = readl(GPIO1_DR) & temp;
	writel(temp, GPIO1_DR);
}

void led_write(unsigned long led_data) {
	led_data = led_data << 16;
	writel(led_data, GPIO1_DR);
}

void led_read(unsigned long* led_value) {
	char ox[7] = {'a', ':', 'a', ':', 'a', ':', 'a'};
	int i = 0;
	unsigned long temp;

	temp = (readl(GPIO1_DR) >> 16) & 0x0f;
	*led_value = temp;
	puts("1:2:3:4\n");
	for(i = 0; i < 4; i++) {
		if((temp & (0x01 << i)) == (0x01 << i))
			ox[i*2] = 'O';
		else
			ox[i*2] = 'X';
	}
	puts(ox);
	puts("\n");
}

static int do_KCCI_LED(cmd_tbl_t* cmdtp, int flag, int argc, char* const argv[]) {
	unsigned long led_data;
	unsigned long led_value;
	if(argc != 2) {
		cmd_usage(cmdtp);
		return 1;
	}
	printf("*LED TEST START(작성자:김지호)\n");
	led_init();
	led_data = simple_strtoul(argv[1], NULL, 16);
	led_write(led_data);
	led_read(&led_value);
	
	printf("*LED TEST END(%s : %#04x)\n\n", argv[0], (unsigned int)led_value);
	return 0;
}

U_BOOT_CMD (
		led, 2, 0, do_KCCI_LED,
		"led - kcci LED TEST.",
		"number - Input argument is only one.(led [0x00~0x0f])\n"
		);

