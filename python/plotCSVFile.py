import csv
import matplotlib.pyplot as plt

def plot_data_from_csv(file_path):
    x_values, y1_values, y2_values = [], [], []

    with open(file_path, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip the first row
        for row in reader:
            x, y1, y2 = map(float, row[0].split()[:3])  # Assuming x, y1 and y2 are the first three elements after splitting by whitespaces
            x_values.append(x)
            y1_values.append(y1)
            y2_values.append(y2)

    fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)
    ax1.plot(x_values, y1_values, 'bo-', label='P (cmH2O)')
    ax2.plot(x_values, y2_values, 'ro-', label='Flow (L/min)')

    integrated = [0]
    new_x_values, new_y2_values = [], []
    for i in range(len(x_values)-1):
        if x_values[i+1] - x_values[i] > 0.1:
            new_x_values.append(x_values[i])
            new_y2_values.append(y2_values[i])
            new_x_values.extend([x_values[i] + (x_values[i+1] - x_values[i]) * j / 10 for j in range(1, 10)])
            new_y2_values.extend([y2_values[i] + (y2_values[i+1] - y2_values[i]) * j / 10 for j in range(1, 10)])
        else:
            new_x_values.append(x_values[i])
            new_y2_values.append(y2_values[i])
    new_x_values.append(x_values[-1])
    new_y2_values.append(y2_values[-1])

    x_values, y2_values = new_x_values, new_y2_values

    for i in range(1, len(x_values)-1):
        integrated.append(integrated[-1] + (x_values[i+1] - x_values[i-1]) * (y2_values[i-1] + 4 * y2_values[i] + y2_values[i+1]) / 6)
    new_integrated = []
    for i in range(len(x_values)):
        new_integrated.append(integrated[min(i, len(integrated)-1)])
    ax2.plot(x_values, new_integrated, 'go-', label='Cumulative Volume')

    ax1.set_title('Pressure')
    ax1.set_ylabel('P')
    ax1.grid(True)
    ax1.legend(loc='upper right')

    ax2.set_title('Flow')
    ax2.set_xlabel('time (s)')
    ax2.set_ylabel('F')
    ax2.grid(True)
    ax2.legend(loc='upper right')

    plt.tight_layout()
    plt.show()

# Example usage
if  __name__ == "__main__":
    fileName = 'ventilator_data2.csv'
    plot_data_from_csv(fileName)

