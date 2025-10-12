#!/usr/bin/env python3
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

# Create graphs directory if it doesn't exist
if not os.path.exists('graphs'):
    os.makedirs('graphs')

print("Generating Order-Statistic Tree Graphs...")

# Check if file exists
if not os.path.exists('data/os_results.csv'):
    print("Error: data/os_results.csv not found!")
    print("Make sure you run the experiments first with: ./bin/os_experiments > data/os_results.csv")
    exit(1)

# Parse the data sections (don't use pd.read_csv for mixed format)
sections = {}
current_section = None

with open('data/os_results.csv', 'r') as f:
    for line in f:
        line = line.strip()
        if line.startswith('==='):
            # Extract section name
            current_section = line.split('===')[1].strip()
            sections[current_section] = []
        elif line and not line.startswith('Experiment') and not line.startswith('n,'):
            # Skip header lines
            if current_section and ',' in line and not line.startswith('==='):
                parts = line.split(',')
                if len(parts) >= 2 and parts[0].isdigit():
                    sections[current_section].append(line)

# Function to parse section data
def parse_section(section_name):
    if section_name not in sections or not sections[section_name]:
        return None

    data_lines = sections[section_name]
    data_dict = {}

    for line in data_lines:
        parts = [p.strip() for p in line.split(',')]
        if len(parts) >= 3:
            try:
                n = int(parts[0])
                if n not in data_dict:
                    data_dict[n] = []
                data_dict[n].extend([float(p) for p in parts[1:]])
            except ValueError:
                continue

    return data_dict

# Graph 1: INSERT Time Comparison (OS-Tree vs BST)
print("Creating INSERT comparison graph...")
insert_data = parse_section('Experiment 1: INSERT Time Comparison (OS-Tree vs BST)')

if insert_data:
    n_values = sorted(insert_data.keys())
    bst_times = [insert_data[n][0] for n in n_values]
    os_times = [insert_data[n][1] for n in n_values]
    overhead = [insert_data[n][2] for n in n_values]

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

    # Subplot 1: Time comparison
    ax1.plot(n_values, bst_times, 'bo-', label='BST Insert', linewidth=2, markersize=6)
    ax1.plot(n_values, os_times, 'ro-', label='OS-Tree Insert', linewidth=2, markersize=6)
    ax1.set_xlabel('Tree Size (n)', fontsize=12)
    ax1.set_ylabel('Time (ms)', fontsize=12)
    ax1.set_title('INSERT Time: OS-Tree vs BST', fontsize=14, fontweight='bold')
    ax1.legend(fontsize=11)
    ax1.grid(True, alpha=0.3)
    ax1.set_xscale('log')
    ax1.set_yscale('log')

    # Subplot 2: Overhead ratio
    ax2.plot(n_values, overhead, 'go-', linewidth=2, markersize=6)
    ax2.axhline(y=1.0, color='r', linestyle='--', label='No overhead (ratio=1)')
    ax2.set_xlabel('Tree Size (n)', fontsize=12)
    ax2.set_ylabel('Overhead Ratio (OS-Tree / BST)', fontsize=12)
    ax2.set_title('OS-Tree INSERT Overhead', fontsize=14, fontweight='bold')
    ax2.legend(fontsize=11)
    ax2.grid(True, alpha=0.3)
    ax2.set_xscale('log')

    plt.tight_layout()
    plt.savefig('graphs/insert_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("✓ Saved: graphs/insert_comparison.png")

# Graph 2: DELETE Time Comparison (OS-Tree vs BST)
print("Creating DELETE comparison graph...")
delete_data = parse_section('Experiment 2: DELETE Time Comparison (OS-Tree vs BST)')

if delete_data:
    n_values = sorted(delete_data.keys())
    bst_times = [delete_data[n][0] for n in n_values]
    os_times = [delete_data[n][1] for n in n_values]
    overhead = [delete_data[n][2] for n in n_values]

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

    # Subplot 1: Time comparison
    ax1.plot(n_values, bst_times, 'bo-', label='BST Delete', linewidth=2, markersize=6)
    ax1.plot(n_values, os_times, 'ro-', label='OS-Tree Delete', linewidth=2, markersize=6)
    ax1.set_xlabel('Tree Size (n)', fontsize=12)
    ax1.set_ylabel('Time (ms)', fontsize=12)
    ax1.set_title('DELETE Time: OS-Tree vs BST', fontsize=14, fontweight='bold')
    ax1.legend(fontsize=11)
    ax1.grid(True, alpha=0.3)
    ax1.set_xscale('log')
    ax1.set_yscale('log')

    # Subplot 2: Overhead ratio
    ax2.plot(n_values, overhead, 'go-', linewidth=2, markersize=6)
    ax2.axhline(y=1.0, color='r', linestyle='--', label='No overhead (ratio=1)')
    ax2.set_xlabel('Tree Size (n)', fontsize=12)
    ax2.set_ylabel('Overhead Ratio (OS-Tree / BST)', fontsize=12)
    ax2.set_title('OS-Tree DELETE Overhead', fontsize=14, fontweight='bold')
    ax2.legend(fontsize=11)
    ax2.grid(True, alpha=0.3)
    ax2.set_xscale('log')

    plt.tight_layout()
    plt.savefig('graphs/delete_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("✓ Saved: graphs/delete_comparison.png")

# Graph 3: OS-SELECT Runtime
print("Creating OS-SELECT runtime graph...")
select_data = parse_section('Experiment 3: OS-SELECT Runtime')

if select_data:
    n_values = sorted(select_data.keys())
    avg_times = [select_data[n][0] for n in n_values]
    per_op_times = [select_data[n][1] for n in n_values]

    # Calculate theoretical O(log n)
    theoretical = [np.log2(n) for n in n_values]
    # Scale to match data
    scale_factor = per_op_times[-1] / theoretical[-1]
    theoretical_scaled = [t * scale_factor for t in theoretical]

    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(n_values, per_op_times, 'bo-', label='OS-Select (measured)', linewidth=2, markersize=6)
    ax.plot(n_values, theoretical_scaled, 'r--', label='O(log n) theoretical', linewidth=2)
    ax.set_xlabel('Tree Size (n)', fontsize=12)
    ax.set_ylabel('Time per Operation (μs)', fontsize=12)
    ax.set_title('OS-SELECT Runtime Analysis', fontsize=14, fontweight='bold')
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.3)
    ax.set_xscale('log')
    ax.set_yscale('log')

    plt.tight_layout()
    plt.savefig('graphs/os_select_runtime.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("✓ Saved: graphs/os_select_runtime.png")

# Graph 4: OS-RANK Runtime
print("Creating OS-RANK runtime graph...")
rank_data = parse_section('Experiment 4: OS-RANK Runtime')

if rank_data:
    n_values = sorted(rank_data.keys())
    avg_times = [rank_data[n][0] for n in n_values]
    per_op_times = [rank_data[n][1] for n in n_values]

    # Calculate theoretical O(log n)
    theoretical = [np.log2(n) for n in n_values]
    scale_factor = per_op_times[-1] / theoretical[-1]
    theoretical_scaled = [t * scale_factor for t in theoretical]

    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(n_values, per_op_times, 'mo-', label='OS-Rank (measured)', linewidth=2, markersize=6)
    ax.plot(n_values, theoretical_scaled, 'r--', label='O(log n) theoretical', linewidth=2)
    ax.set_xlabel('Tree Size (n)', fontsize=12)
    ax.set_ylabel('Time per Operation (μs)', fontsize=12)
    ax.set_title('OS-RANK Runtime Analysis', fontsize=14, fontweight='bold')
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.3)
    ax.set_xscale('log')
    ax.set_yscale('log')

    plt.tight_layout()
    plt.savefig('graphs/os_rank_runtime.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("✓ Saved: graphs/os_rank_runtime.png")

# Graph 5: Combined OS operations
print("Creating combined OS operations graph...")
if select_data and rank_data:
    n_values = sorted(select_data.keys())
    select_times = [select_data[n][1] for n in n_values]
    rank_times = [rank_data[n][1] for n in n_values]

    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(n_values, select_times, 'bo-', label='OS-Select', linewidth=2, markersize=6)
    ax.plot(n_values, rank_times, 'mo-', label='OS-Rank', linewidth=2, markersize=6)

    # Theoretical O(log n)
    theoretical = [np.log2(n) for n in n_values]
    scale_factor = (select_times[-1] + rank_times[-1]) / (2 * theoretical[-1])
    theoretical_scaled = [t * scale_factor for t in theoretical]
    ax.plot(n_values, theoretical_scaled, 'r--', label='O(log n) reference', linewidth=2)

    ax.set_xlabel('Tree Size (n)', fontsize=12)
    ax.set_ylabel('Time per Operation (μs)', fontsize=12)
    ax.set_title('Order-Statistic Operations: OS-Select & OS-Rank', fontsize=14, fontweight='bold')
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.3)
    ax.set_xscale('log')
    ax.set_yscale('log')

    plt.tight_layout()
    plt.savefig('graphs/os_operations_combined.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("✓ Saved: graphs/os_operations_combined.png")

print("\n✓ All OS-Tree graphs generated successfully!")
print("Check the 'graphs/' directory for the following:")
print("  1. os_insert_comparison.png - INSERT overhead analysis")
print("  2. os_delete_comparison.png - DELETE overhead analysis")
print("  3. os_select_runtime.png - OS-SELECT O(log n) verification")
print("  4. os_rank_runtime.png - OS-RANK O(log n) verification")
print("  5. os_operations_combined.png - Both operations comparison")
