#include"hamming.h"

int main(int argc, char **argv)
{
	int data[7], rec[7], i, c1, c2, c3, c;

	std::cout<<"\n\nHamming(7, 4) Encoding & Decoding.\nEnter message bit one after another (MSB first)\n";
	std::cin>>data[4]>>data[2]>>data[1]>>data[0];
	std::cout<<"\nEntered message: " << data[4] << data[2] << data[1] << data[0];

	data[3] = data[0] ^ data[1] ^ data[2];
	data[5] = data[0] ^ data[1] ^ data[4];
	data[6] = data[0] ^ data[2] ^ data[4];
	
	std::cout << "\nEncoded message: ";
	
	for (i = 6;i>=0;i--)
	{
		std::cout<<data[i];
	}

	std::cout<<"\nEnter received data bits one after another (MSB first)\n";

	for (i = 6;i>=0;i--)
	{
		std::cin>>rec[i];
	}

	std::cout << "\nReceived data bits: ";
	for (i = 6;i >= 0;i--)
	{
		std::cout << rec[i];
	}
	c1 = rec[6] ^ rec[4] ^ rec[2] ^ rec[0];
	c2 = rec[5] ^ rec[4] ^ rec[1] ^ rec[0];
	c3 = rec[3] ^ rec[2] ^ rec[1] ^ rec[0];
	c = c3 * 4 + c2 * 2 + c1;
	if (c == 0)
	{
		std::cout<<"\nNo error detected\n";
	}
	else
	{
		std::cout<<"\nError on the postion: "<<7-c+1<<"\nCorrected message is \n";
		if (rec[7 - c] == 0)
		{
			rec[7 - c] = 1;
		}
		else
		{
			rec[7 - c] = 0;
		}

		for (i = 6;i>=0;i--)
		{
			std::cout<<rec[i];
		}
	}

	std::cout << "\nDecoded message after correction: " << rec[4] << rec[2] << rec[1] << rec[0] << "\n\n";

	getchar();
}