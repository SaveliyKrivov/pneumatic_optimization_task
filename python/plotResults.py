import matplotlib.pyplot as plt

def plot_data_from_file(file_path):
    # Save first two strings from the file
    initParamsStr = ""
    optParamsStr = ""
    with open(file_path, 'r') as file:
        targetParams = file.readline().strip()
        optParamsStr = file.readline().strip()
        initialParamsStr = file.readline().strip()

    # Read data from file
    time, F, P, Popt, Pinit, _ = [], [], [], [], [], []
    with open(file_path, 'r') as file:
        for line in file.readlines()[2:]:
            t, x, y, y1, pi, _ = map(float, line.split())
            time.append(t)
            F.append(x)
            P.append(y)
            Popt.append(y1)
            Pinit.append(pi)

    # Plot the data
    fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)

    ax1.plot(time, Popt, label="Popt " + optParamsStr)
    ax1.plot(time, P, 'ro', label="Pdata")
    ax1.set_ylabel('pressure (cmH2O)')
    ax1.legend()
    ax1.set_title('Parameters fitting result')

    ax2.plot(time, Pinit, label="Pinit " + initialParamsStr)
    ax2.plot(time, P, 'ro', label="Pdata")
    ax2.set_title('Initial guess "')
    ax2.set_xlabel('time (s)')
    ax2.set_ylabel('pressure (cmH2O)')
    ax2.legend()
    
    plt.show()

if __name__ == "__main__":
    plot_data_from_file('output.txt')

