import serial

print "Beginning System Identification"

# ----- FOR MACS ONLY -----
# ser = serial.Serial('/dev/tty.usbmodem1411', 9600)

# ----- FOR WINDOWS ONLY -----
ser = serial.Serial('COM5', 9600)

# ----- KEEP NUM_SAMPLES UPDATED -----
NUM_SAMPLES = 1000

i = 0
sample = []
error = []

print "Beginning Sampling"

# Reading in the values from the arduino and putting them into lists
while i < NUM_SAMPLES:
    line = ser.readline()
    hold = line.split(",")
    sample.append(hold[0])
    error.append(hold[1])
    print hold[0]
    print hold[1]
    i += 1

# Writing the values to the csv file
f = open('CollectedValues.csv', 'w')
for (s, e) in zip(sample, error):
    f.write(s + ',' + e + ',\n')
f.close()

print "Finished Sampling"
