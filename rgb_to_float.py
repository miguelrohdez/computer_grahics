def rgb_to_float(numbers):
	floats = []
	for number in numbers:
		floats.append(number/255)

	return floats

def main():
	flag = 0
	while flag != 1:
		rgb = input("RGB separate by comas>> ")
		if rgb == "":
			flag = 1
		else:			
			rgb = rgb.replace(" ", "")
			rgb = rgb.split(",")
			rgb = [int(number) for number in rgb]
			floats = rgb_to_float(rgb)
			print("\tglColor3f(" + '{:.2f}'.format(floats[0]) + "f, " + '{:.2f}'.format(floats[1]) + "f, " + '{:.2f}'.format(floats[2])+"f);\n")			


if __name__ == '__main__':
	main()