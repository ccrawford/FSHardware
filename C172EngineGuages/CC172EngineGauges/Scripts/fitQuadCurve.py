#!/usr/bin/env python3

import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import argparse

def quad_func(x, a, b, c):
    """Quadratic function for fitting."""
    return a * x**2 + b * x + c

def create_fit():
    """Create the fitting function based on known data points."""
    # Known data points
    temp = np.array([75, 100, 150, 200, 245])
    angles = np.array([66, 49, 28, -34, -60])
    
    # Fit the function to the data
    popt, _ = curve_fit(quad_func, temp, angles)
    return popt

def temp_to_angle(temperature, coefficients):
    """Convert temperature to gauge angle using fitted function."""
    return quad_func(temperature, *coefficients)

def plot_fit(coefficients, save_path=None):
    """Create and optionally save a plot of the fit."""
    # Original data points
    temp = np.array([75, 100, 150, 200, 245])
    angles = np.array([66, 49, 28, -34, -60])
    
    # Generate smooth curve
    temp_smooth = np.linspace(min(temp), max(temp), 100)
    angles_smooth = temp_to_angle(temp_smooth, coefficients)
    
    # Create plot
    plt.figure(figsize=(10, 6))
    plt.scatter(temp, angles, color='red', label='Original data')
    plt.plot(temp_smooth, angles_smooth, color='blue', label='Fitted curve')
    plt.grid(True)
    plt.xlabel('Temperature (°F)')
    plt.ylabel('Angle (degrees)')
    plt.title('Temperature to Gauge Angle Conversion')
    plt.legend()
    
    if save_path:
        plt.savefig(save_path)
        plt.close()
    else:
        plt.show()

def main():
    parser = argparse.ArgumentParser(description='Convert temperature to gauge angle')
    parser.add_argument('temperature', type=float, nargs='?', 
                      help='Temperature value to convert (optional)')
    parser.add_argument('--plot', action='store_true',
                      help='Show the fit plot')
    parser.add_argument('--save-plot', type=str, metavar='FILE',
                      help='Save the fit plot to a file')
    parser.add_argument('--show-all', action='store_true',
                      help='Show conversions for all original calibration points')
    
    args = parser.parse_args()
    
    # Get the fitted coefficients
    coefficients = create_fit()
    a, b, c = coefficients
    
    # Print the function
    print(f"\nFitted function: angle = {a:.6f}x² + {b:.6f}x + {c:.6f}")
    print("\nWhere x is the temperature in °F and angle is in degrees")
    print("\nValid temperature range: 75°F to 245°F")
    
    # Show conversion for specific temperature if provided
    if args.temperature is not None:
        if 75 <= args.temperature <= 245:
            angle = temp_to_angle(args.temperature, coefficients)
            print(f"\nTemperature: {args.temperature}°F -> Angle: {angle:.1f}°")
        else:
            print(f"\nWarning: Temperature {args.temperature}°F is outside the calibrated range (75-245°F)")
            print("Result may be unreliable:")
            angle = temp_to_angle(args.temperature, coefficients)
            print(f"Temperature: {args.temperature}°F -> Angle: {angle:.1f}°")
    
    # Show all calibration points if requested
    if args.show_all:
        print("\nOriginal calibration points:")
        for temp in [75, 100, 150, 200, 245]:
            angle = temp_to_angle(temp, coefficients)
            print(f"Temperature: {temp}°F -> Angle: {angle:.1f}°")
    
    # Handle plotting
    if args.save_plot:
        plot_fit(coefficients, args.save_plot)
        print(f"\nPlot saved to: {args.save_plot}")
    elif args.plot:
        plot_fit(coefficients)

if __name__ == "__main__":
    main()