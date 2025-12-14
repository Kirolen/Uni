import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

def read_from_file(filename: str = 'f4.txt') -> np.ndarray | None:
    # This function reads data from a file and returns it as a NumPy array.
    try:
        data = np.loadtxt(filename)
        return data
    except FileNotFoundError:
        print(f"File with name '{filename}' not found.")
        return None

def DFT(signal: np.ndarray) -> np.ndarray:
    # Computes the Discrete Fourier Transform (DFT) of a given signal.
    signal = np.asarray(signal, dtype=float)
    N = signal.shape[0]
    c_x = np.zeros(N, dtype=complex)
    
    for k in range(N):
        c_x[k] = np.sum(signal * np.exp(-2j * np.pi * k * np.arange(N) / N))
    
    return c_x

def test_dft_implementation(samples: np.ndarray) -> None:
    # Tests whether the custom DFT implementation matches NumPy's FFT.
    my_dft_result = DFT(samples)
    numpy_fft_result = np.fft.fft(samples)
    max_abs_error = np.max(np.abs(my_dft_result - numpy_fft_result))

    if max_abs_error < 1e-6:
        print("DFT implementation matches NumPy's FFT.")
    else:
        print("DFT implementation does not match NumPy's FFT.")

def find_peaks(amplitude: np.ndarray, threshold_ratio: float = 0.1) -> np.ndarray:
    # Finding local amplitude maxima with a threshold.
    N = len(amplitude)
    half_N = N // 2
    amp = amplitude[:half_N]
    threshold = threshold_ratio * np.max(amp)

    peaks = [i for i in range(1, half_N - 1)
             if amp[i] > amp[i - 1] and amp[i] > amp[i + 1] and amp[i] >= threshold]
    
    return np.array(peaks)

def analyze_DFS(dft: np.ndarray, dt: float) -> tuple[np.ndarray, np.ndarray]:
    # Analyzes the spectrum: amplitude and frequency.
    amplitude = np.abs(dft)
    frequencies = np.fft.fftfreq(len(dft), dt)
    peaks = find_peaks(amplitude)
    significant_frequencies = frequencies[peaks]
    return amplitude, significant_frequencies

def fit_model(t, observations, peak_frequencies):
    # Fits a signal model (cubic polynomial + harmonics) to the observed data using least squares
    def model(t, a1, a2, a3, *params):
        # Inner function to build the signal model with a cubic polynomial and harmonics
        y = a1*t**3 + a2*t**2 + a3*t
        k = len(peak_frequencies)
        for i in range(k):
            ai = params[i]           
            phi_i = params[k + i]    
            fi = peak_frequencies[i] 
            y += ai * np.sin(2*np.pi*fi*t + phi_i)
        return y

    k = len(peak_frequencies)
    initial_guess = [1, 1, 1] + [1]*k + [0]*k 
    params, _ = curve_fit(lambda t, *p: model(t, *p), t, observations, p0=initial_guess)
    return params, model(t, *params)

def plot_amplitude_spectrum(amplitude, dt):
    # A 'Amplitude spectrum of the signal' plotting function
    N = len(amplitude)
    frequencies = np.fft.fftfreq(N, dt)
    half_N = N // 2
    plt.figure(figsize=(10,5))
    plt.plot(frequencies[:half_N], amplitude[:half_N])
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Amplitude')
    plt.title('Amplitude spectrum of the signal')
    plt.grid(True)
    plt.show()

def plot_signal_vs_model(t, observations, model_values):
    # A 'signal vs model' plotting function
    plt.figure(figsize=(10,5))
    plt.plot(t, observations, label='Original')
    plt.plot(t, model_values, '--', color='red', label='Model')
    plt.xlabel('Time t')
    plt.ylabel('y(t)')
    plt.title('Comparison of signal and model')
    plt.legend()
    plt.grid(True)
    plt.show()


def main():
    data = read_from_file()
    dt = 0.01

    if data is not None:
        N = len(data)
        t = np.arange(0, N*dt, dt)

        c_x = DFT(data)

        print("Checking DFT against np.fft.fft:")
        test_dft_implementation(data)

        amplitude, significant_freqs = analyze_DFS(c_x, dt)
        print("\nAmplitude spectrum (first 10 values):", amplitude[:10])
        print("\nDetected significant frequencies:", significant_freqs)

        plot_amplitude_spectrum(amplitude, dt)

        params, model_values = fit_model(t, data, significant_freqs)
        print("\nFitted model parameters:", params)
        plot_signal_vs_model(t, data, model_values)


if __name__ == "__main__":
    main()
