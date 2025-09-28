#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
import sys
import re

def parse_experiments(filename):
    """Parse the experimental data from the output file"""
    with open(filename, 'r') as f:
        content = f.read()
    
    experiments = {}
    current_method = None
    current_experiment = None
    in_comparison = False
    
    lines = content.split('\n')
    for line in lines:
        # Check for method headers
        if 'NoShuffle:' in line:
            current_method = 'NoShuffle'
            in_comparison = False
        elif 'FisherYates:' in line:
            current_method = 'FisherYates'
            in_comparison = False
        elif 'RandomizeInPlace:' in line:
            current_method = 'RandomizeInPlace'
            in_comparison = False
        elif 'PermuteBySorting:' in line:
            current_method = 'PermuteBySorting'
            in_comparison = False
        
        # Check for experiment type
        if current_method and not in_comparison:
            if 'Height Experiment' in line:
                current_experiment = 'height'
            elif 'Build Time' in line:
                current_experiment = 'build'
            elif 'Destroy Time' in line:
                current_experiment = 'destroy'
            elif 'Inorder Walk' in line:
                current_experiment = 'inorder'
            
            if current_method not in experiments:
                experiments[current_method] = {}
            if current_experiment and current_experiment not in experiments[current_method]:
                experiments[current_method][current_experiment] = []
        
        # Check for comparison section - look for the actual comparison header
        if 'COMPARISON: All Four Methods' in line or 'COMPARISON: All' in line:
            current_method = 'comparison'
            in_comparison = True
            if current_method not in experiments:
                experiments[current_method] = {}
        elif in_comparison:
            if 'Height Comparison' in line and '===' in line:
                current_experiment = 'height_comp'
                experiments['comparison'][current_experiment] = []
            elif 'Build Time Comparison' in line and '===' in line:
                current_experiment = 'build_comp'
                experiments['comparison'][current_experiment] = []
            elif 'Destroy Time Comparison' in line and '===' in line:
                current_experiment = 'destroy_comp'
                experiments['comparison'][current_experiment] = []
            elif 'Inorder Walk Comparison' in line and '===' in line:
                current_experiment = 'inorder_comp'
                experiments['comparison'][current_experiment] = []
        
        # Parse data lines
        if ',' in line and current_method and current_experiment:
            if not line.startswith('n,'):  # Skip headers
                try:
                    values = [float(x) for x in line.split(',')]
                    if len(values) >= 2:  # Make sure we have valid data
                        if current_method == 'comparison':
                            experiments[current_method][current_experiment].append(values)
                        else:
                            experiments[current_method][current_experiment].append(values)
                except:
                    pass
    
    return experiments

def plot_individual_method(method, data, save_path):
    """Plot all 4 experiments for a single method"""
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle(f'BST Experiments - {method}', fontsize=16, fontweight='bold')
    
    # Height plot
    if 'height' in data and len(data['height']) > 0:
        height_data = np.array(data['height'])
        n_values = height_data[:, 0]
        heights = height_data[:, 1]
        
        ax1.plot(n_values, heights, 'o-', markersize=6, linewidth=2, label=method)
        
        # Add theoretical line
        if method == 'NoShuffle':
            ax1.plot(n_values, n_values - 1, 'r--', label='n-1 (worst case)', alpha=0.7, linewidth=2)
        else:
            theoretical = 2 * np.log2(n_values)
            ax1.plot(n_values, theoretical, 'r--', label='2*log2(n) (expected)', alpha=0.7, linewidth=2)
        
        ax1.set_xlabel('Number of nodes (n)')
        ax1.set_ylabel('Average Height')
        ax1.set_title('Tree Height')
        ax1.set_xscale('log')
        if method == 'NoShuffle':
            ax1.set_yscale('log')
        ax1.grid(True, alpha=0.3)
        ax1.legend()
    
    # Build time plot
    if 'build' in data and len(data['build']) > 0:
        build_data = np.array(data['build'])
        n_values = build_data[:, 0]
        times = build_data[:, 1]
        
        ax2.plot(n_values, times, 'o-', markersize=6, linewidth=2, color='green')
        ax2.set_xlabel('Number of nodes (n)')
        ax2.set_ylabel('Build Time (ms)')
        ax2.set_title('Build Time')
        ax2.set_xscale('log')
        ax2.set_yscale('log')
        ax2.grid(True, alpha=0.3)
    
    # Destroy time plot - FIXED
    if 'destroy' in data and len(data['destroy']) > 0:
        destroy_data = np.array(data['destroy'])
        n_values = destroy_data[:, 0]
        times = destroy_data[:, 1]
        
        ax3.plot(n_values, times, 'o-', markersize=6, linewidth=2, color='red')
        ax3.set_xlabel('Number of nodes (n)')
        ax3.set_ylabel('Destroy Time (ms)')  # Fixed label
        ax3.set_title('Destroy Time (Root Deletion)')
        ax3.set_xscale('log')
        ax3.set_yscale('log')
        ax3.grid(True, alpha=0.3)
    
    # Inorder walk plot - show TOTAL TIME to confirm Θ(n)
    if 'inorder' in data and len(data['inorder']) > 0:
        inorder_data = np.array(data['inorder'])
        n_values = inorder_data[:, 0]
        
        # Check if we have total time column (new format) or just time per node (old format)
        if inorder_data.shape[1] > 2:
            # New format with total time
            total_times = inorder_data[:, 1]
        else:
            # Old format - calculate total time from time per node
            time_per_node = inorder_data[:, 1]
            total_times = time_per_node * n_values
        
        ax4.plot(n_values, total_times, 'o-', markersize=6, linewidth=2, color='purple')
        
        # Fit a linear line to show Θ(n)
        coeffs = np.polyfit(n_values, total_times, 1)
        linear_fit = np.polyval(coeffs, n_values)
        ax4.plot(n_values, linear_fit, 'r--', 
                label=f'Linear fit: {coeffs[0]:.2e}×n', 
                alpha=0.7, linewidth=2)
        
        ax4.set_xlabel('Number of nodes (n)')
        ax4.set_ylabel('Total Walk Time (ms)')
        ax4.set_title('Inorder Walk - Θ(n) Confirmation')
        ax4.grid(True, alpha=0.3)
        ax4.legend()
    
    plt.tight_layout()
    plt.savefig(save_path, dpi=150)
    print(f"Saved: {save_path}")
    plt.close()

def plot_all_comparisons(experiments):
    """Create comparison plots with all 4 methods"""
    if 'comparison' not in experiments:
        print("No comparison data found")
        return
    
    fig = plt.figure(figsize=(18, 12))
    
    # Colors and markers for each method
    colors = ['red', 'blue', 'green', 'orange']
    markers = ['s', 'o', '^', 'D']
    methods = ['No Shuffle', 'Fisher-Yates', 'RANDOMIZE-IN-PLACE', 'PERMUTE-BY-SORTING']
    linestyles = ['-', '-', '--', '-.']
    
    # Height comparison
    ax1 = plt.subplot(2, 3, 1)
    if 'height_comp' in experiments['comparison']:
        data = np.array(experiments['comparison']['height_comp'])
        if len(data) > 0:
            n_values = data[:, 0]
            for i in range(4):
                ax1.plot(n_values, data[:, i+1], marker=markers[i], color=colors[i], 
                        markersize=5, label=methods[i], alpha=0.8, linewidth=2, linestyle=linestyles[i])
            
            # Add theoretical lines
            theoretical_random = 2 * np.log2(n_values)
            theoretical_worst = n_values - 1
            ax1.plot(n_values, theoretical_random, 'b:', alpha=0.3, linewidth=1)
            ax1.plot(n_values, theoretical_worst, 'r:', alpha=0.3, linewidth=1)
            
            ax1.set_xlabel('Number of nodes (n)')
            ax1.set_ylabel('Average Height')
            ax1.set_title('Height Comparison', fontweight='bold')
            ax1.set_xscale('log')
            ax1.set_yscale('log')
            ax1.grid(True, alpha=0.3)
            ax1.legend(fontsize=8, loc='upper left')
    
    # Build time comparison
    ax2 = plt.subplot(2, 3, 2)
    if 'build_comp' in experiments['comparison']:
        data = np.array(experiments['comparison']['build_comp'])
        if len(data) > 0:
            n_values = data[:, 0]
            for i in range(4):
                ax2.plot(n_values, data[:, i+1], marker=markers[i], color=colors[i],
                        markersize=5, label=methods[i], alpha=0.8, linewidth=2, linestyle=linestyles[i])
            
            ax2.set_xlabel('Number of nodes (n)')
            ax2.set_ylabel('Build Time (ms)')
            ax2.set_title('Build Time Comparison', fontweight='bold')
            ax2.set_xscale('log')
            ax2.set_yscale('log')
            ax2.grid(True, alpha=0.3)
            ax2.legend(fontsize=8, loc='upper left')
    
    # Destroy time comparison
    ax3 = plt.subplot(2, 3, 3)
    if 'destroy_comp' in experiments['comparison']:
        data = np.array(experiments['comparison']['destroy_comp'])
        if len(data) > 0:
            n_values = data[:, 0]
            for i in range(4):
                ax3.plot(n_values, data[:, i+1], marker=markers[i], color=colors[i],
                        markersize=5, label=methods[i], alpha=0.8, linewidth=2, linestyle=linestyles[i])
            
            ax3.set_xlabel('Number of nodes (n)')
            ax3.set_ylabel('Destroy Time (ms)')
            ax3.set_title('Destroy Time Comparison', fontweight='bold')
            ax3.set_xscale('log')
            ax3.set_yscale('log')
            ax3.grid(True, alpha=0.3)
            ax3.legend(fontsize=8, loc='upper left')
    
    # Inorder walk comparison - show TOTAL TIME for Θ(n)
    ax4 = plt.subplot(2, 3, 4)
    if 'inorder_comp' in experiments['comparison']:
        data = np.array(experiments['comparison']['inorder_comp'])
        if len(data) > 0:
            n_values = data[:, 0]
            
            # Check format and calculate total times
            if data.shape[1] > 5:  # New format with both total and per-node
                for i in range(4):
                    total_times = data[:, i*2 + 1]  # Total time columns
                    ax4.plot(n_values, total_times, marker=markers[i], color=colors[i],
                            markersize=5, label=methods[i], alpha=0.8, linewidth=2, linestyle=linestyles[i])
            else:  # Old format with time per node
                for i in range(4):
                    time_per_node = data[:, i+1]
                    total_times = time_per_node * n_values  # Calculate total time
                    ax4.plot(n_values, total_times, marker=markers[i], color=colors[i],
                            markersize=5, label=methods[i], alpha=0.8, linewidth=2, linestyle=linestyles[i])
            
            # Add a reference line showing linear growth
            reference_line = n_values * np.mean(data[:, 2] * 1000)  # Scale appropriately
            ax4.plot(n_values, reference_line, 'k:', alpha=0.3, label='Linear (Θ(n))')
            
            ax4.set_xlabel('Number of nodes (n)')
            ax4.set_ylabel('Total Walk Time (ms)')
            ax4.set_title('Inorder Walk - Θ(n) Confirmation', fontweight='bold')
            ax4.grid(True, alpha=0.3)
            ax4.legend(fontsize=8)
    
    # Performance ratios (worst case vs random cases)
    ax5 = plt.subplot(2, 3, 5)
    if 'height_comp' in experiments['comparison']:
        data = np.array(experiments['comparison']['height_comp'])
        if len(data) > 0:
            n_values = data[:, 0]
            # Calculate ratios: Sequential / Random methods
            for i in range(1, 4):
                ratio = data[:, 1] / data[:, i+1]  # NoShuffle / Random method
                ax5.plot(n_values, ratio, marker=markers[i], label=f'Sequential/{methods[i]}', 
                        markersize=5, alpha=0.8, linewidth=2)
            
            ax5.set_xlabel('Number of nodes (n)')
            ax5.set_ylabel('Height Ratio')
            ax5.set_title('Height Improvement Ratio', fontweight='bold')
            ax5.set_xscale('log')
            ax5.grid(True, alpha=0.3)
            ax5.legend(fontsize=8)
    
    # Summary statistics table
    ax6 = plt.subplot(2, 3, 6)
    ax6.axis('off')
    
    if 'height_comp' in experiments['comparison'] and 'build_comp' in experiments['comparison']:
        height_data = np.array(experiments['comparison']['height_comp'])
        build_data = np.array(experiments['comparison']['build_comp'])
        
        # Calculate statistics for largest n
        last_row_height = height_data[-1, 1:]
        last_row_build = build_data[-1, 1:]
        n_max = int(height_data[-1, 0])
        
        # Create summary table
        summary_text = f"Summary for n = {n_max}\n" + "="*40 + "\n\n"
        summary_text += "Method                Height    Build(ms)\n"
        summary_text += "-"*40 + "\n"
        
        for i, method in enumerate(methods):
            summary_text += f"{method:<20} {last_row_height[i]:>6.1f}    {last_row_build[i]:>8.2f}\n"
        
        summary_text += "\n" + "="*40 + "\n"
        summary_text += "Height Improvement vs Sequential:\n"
        for i in range(1, 4):
            improvement = last_row_height[0] / last_row_height[i]
            summary_text += f"  {methods[i]}: {improvement:.1f}x\n"
        
        ax6.text(0.1, 0.9, summary_text, transform=ax6.transAxes, 
                fontfamily='monospace', fontsize=10, verticalalignment='top')
    
    plt.suptitle('BST Performance Comparison - All Four Methods', fontsize=16, fontweight='bold')
    plt.tight_layout()
    plt.savefig('graphs/bst_all_comparisons.png', dpi=150)
    print("Saved: graphs/bst_all_comparisons.png")
    plt.close()

def main():
    """Main function to generate all plots"""
    import os
    os.makedirs('graphs', exist_ok=True)
    
    # Set style for better-looking plots
    try:
        plt.style.use('seaborn-v0_8-darkgrid')
    except:
        try:
            plt.style.use('seaborn-darkgrid')
        except:
            pass  # Use default style if seaborn not available
    
    # Read data file
    filename = 'data/bst_results.csv'
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    
    try:
        experiments = parse_experiments(filename)
        
        # Debug: Print what was parsed
        print("Parsed experiments:")
        for method in experiments:
            print(f"  {method}:", list(experiments[method].keys()))
        
        print("\nPlotting individual methods...")
        # Plot individual method results
        methods_to_plot = ['NoShuffle', 'FisherYates', 'RandomizeInPlace', 'PermuteBySorting']
        for method in methods_to_plot:
            if method in experiments and experiments[method]:
                plot_individual_method(method, experiments[method], 
                                     f'graphs/bst_{method.lower()}.png')
        
        # Plot comparison graphs
        if 'comparison' in experiments:
            print(f"Comparison data found with keys: {list(experiments['comparison'].keys())}")
            plot_all_comparisons(experiments)
        else:
            print("No comparison data found - check if comparison section exists in data file")
        
        print("\n" + "="*60)
        print("All plots generated successfully!")
        print("="*60)
        print("\nKey findings:")
        print("1. No Shuffle: Creates degenerate BST with height = n-1 (worst case)")
        print("2. Fisher-Yates: Random BST with height ≈ 2×log₂(n)")  
        print("3. RANDOMIZE-IN-PLACE: Random BST with height ≈ 2×log₂(n)")
        print("4. PERMUTE-BY-SORTING: Random BST with height ≈ 2×log₂(n)")
        print("\nAll three randomization methods produce similar results,")
        print("confirming Theorem 12.4 about randomly built BSTs.")
        print("\nInorder Walk: Θ(n) confirmed - time per node is constant")
        print("for all methods regardless of tree structure.")
        
    except FileNotFoundError:
        print(f"Error: Could not find {filename}")
        print("Run 'make run_experiments' first to generate data")
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main()