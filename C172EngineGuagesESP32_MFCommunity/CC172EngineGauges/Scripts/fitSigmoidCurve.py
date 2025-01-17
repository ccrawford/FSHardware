#!/usr/bin/env python3

import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import argparse

def quad_func(x, a, b, c):
    """Quadratic function for fitting."""
    return a * x**2 + b * x + c

def sigmoid_func(x, L, k, x0, b):
    """Modified sigmoid function for fitting.
    L: maximum value
    k: steepness
    x0: midpoint
    b: vertical shift
    """
    return L / (1 + np.exp(-k * (x - x0))) + b

def create_fits():
    """Create both quadratic and sigmoid fitting functions based on known data points."""
    # Known data points
    temp = np.array([75, 100, 150, 200, 245])
    angles = np.array([66, 49, 28, -34, -60])
    
    # Fit quadratic function
    quad_popt, _ = curve_fit(quad_func, temp, angles)
    
    # Fit sigmoid function with initial guesses
    sigmoid_popt, _ = curve_fit(sigmoid_func, temp, angles, 
                               p0=[-130, -0.03, 150, 70])  # Initial parameter guesses
    
    return quad_popt, sigmoid_popt

def temp_to_angle(temperature, coefficients, func_type='sigmoid'):
    """Convert temperature to gauge angle using fitted function."""
    if func_type == 'quadratic':
        return quad_func(temperature, *coefficients)
    else:
        return sigmoid_func(temperature, *coefficients)

def plot_fits(quad_coefficients, sigmoid_coefficients, save_path=None):
    """Create and optionally save a plot comparing both fits."""
    # Original data points
    temp = np.array([75, 100, 150, 200, 245])
    angles = np.array([66, 49, 28, -34, -60])
    
    # Generate smooth curves
    temp_smooth = np.linspace(min(temp), max(temp), 100)
    angles_quad = temp_to_angle(temp_smooth, quad_coefficients, 'quadratic')
    angles_sigmoid = temp_to_angle(temp_smooth, sigmoid_coefficients, 'sigmoid')
    
    # Create plot
    plt.figure(figsize=(12, 8))
    plt.scatter(temp, angles, color='red', label='Original data', s=100)
    plt.plot(temp_smooth, angles_quad, '--', color='blue', label='Quadratic fit')
    plt.plot(temp_smooth, angles_sigmoid, color='green', label='Sigmoid fit')
    plt.grid(True)
    plt.xlabel('Temperature (°F)')
    plt.ylabel('Angle (degrees)')
    plt.title('Temperature to Gauge Angle Conversion - Comparing Fits')
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
                      help='Show the fit plots')
    parser.add_argument('--save-plot', type=str, metavar='FILE',
                      help='Save the fit plot to a file')
    parser.add_argument('--show-all', action='store_true',
                      help='Show conversions for all original calibration points')
    parser.add_argument('--function', choices=['quadratic', 'sigmoid'],
                      default='sigmoid', help='Choose fitting function (default: sigmoid)')
    
    args = parser.parse_args()
    
    # Get the fitted coefficients
    quad_coefficients, sigmoid_coefficients = create_fits()
    
    coefficients = quad_coefficients if args.function == 'quadratic' else sigmoid_coefficients
    
    # Print the functions
    if args.function == 'quadratic':
        a, b, c = coefficients
        print(f"\nQuadratic function: angle = {a:.6f}x² + {b:.6f}x + {c:.6f}")
    else:
        L, k, x0, b = coefficients
        print(f"\nSigmoid function parameters:")
        print(f"L (amplitude): {L:.6f}")
        print(f"k (steepness): {k:.6f}")
        print(f"x0 (midpoint): {x0:.6f}")
        print(f"b (offset): {b:.6f}")
    
    print("\nWhere x is the temperature in °F and angle is in degrees")
    print("\nValid temperature range: 75°F to 245°F")
    
    # Show conversion for specific temperature if provided
    if args.temperature is not None:
        if 75 <= args.temperature <= 245:
            angle = temp_to_angle(args.temperature, coefficients, args.function)
            print(f"\nTemperature: {args.temperature}°F -> Angle: {angle:.1f}°")
        else:
            print(f"\nWarning: Temperature {args.temperature}°F is outside the calibrated range (75-245°F)")
            print("Result may be unreliable:")
            angle = temp_to_angle(args.temperature, coefficients, args.function)
            print(f"Temperature: {args.temperature}°F -> Angle: {angle:.1f}°")
    
    # Show all calibration points if requested
    if args.show_all:
        print("\nOriginal calibration points:")
        for temp in [75, 100, 150, 200, 245]:
            angle = temp_to_angle(temp, coefficients, args.function)
            print(f"Temperature: {temp}°F -> Angle: {angle:.1f}°")
    
    # Handle plotting
    if args.save_plot:
        plot_fits(quad_coefficients, sigmoid_coefficients, args.save_plot)
        print(f"\nPlot saved to: {args.save_plot}")
    elif args.plot:
        plot_fits(quad_coefficients, sigmoid_coefficients)

if __name__ == "__main__":
    main()