#!/usr/bin/env python3
"""
Graph generation script for BST and OS-Tree experiments
Creates publication-quality graphs from CSV data
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import sys

# Set style for better-looking plots
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (10, 6)
plt.rcParams['font.size'] = 12

def ensure_directories():
    """Create necessary directories if they don't exist"""
    os.makedirs('graphs', exist_ok=True)
    os.makedirs('data', exist_ok=True)

def plot_bst_heights():
    """Plot BST height analysis"""
    try:
        df = pd.read_csv('data/bst_heights.csv')
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
        
        # Plot 1: Average height vs theoretical bounds
        ax1.plot(df['size'], df['avg_height'], 'bo-', label='Average Height', linewidth=2, markersize=8)
        ax1.fill_between(df['size'], df['min_height'], df['max_height'], alpha=0.3, label='Min-Max Range')
        
        # Add theoretical bounds
        sizes = df['size'].values
        log_n = np.log2(sizes)
        sqrt_n = np.sqrt(sizes)
        
        ax1.plot(sizes, log_n, 'r--', label='log₂(n)', linewidth=2)
        ax1.plot(sizes, 2.99 * log_n, 'g--', label='2.99×log₂(n)', linewidth=2)
        
        ax1.set_xlabel('Number of Nodes (n)')
        ax1.set_ylabel('Tree Height')
        ax1.set_title('BST Height vs Tree Size')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        ax1.set_xscale('log')
        
        # Plot 2: Height ratio to log(n)
        ratio = df['avg_height'] / np.log2(df['size'])
        ax2.plot(df['size'], ratio, 'mo-', linewidth=2, markersize=8)
        ax2.axhline(y=1, color='r', linestyle='--', label='Ratio = 1')
        ax2.axhline(y=2.99, color='g', linestyle='--', label='Ratio = 2.99 (theoretical)')
        
        ax2.set_xlabel('Number of Nodes (n)')
        ax2.set_ylabel('Height / log₂(n)')
        ax2.set_title('Height Ratio to log₂(n)')
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        ax2.set_xscale('log')
        
        plt.tight_layout()
        plt.savefig('graphs/bst_heights.png', dpi=300, bbox_inches='tight')
        print("✓ Generated: bst_heights.png")
        
    except FileNotFoundError:
        print("✗ Cannot find bst_heights.csv - run BST experiments first")
    except Exception as e:
        print(f"✗ Error plotting BST heights: {e}")

def plot_bst_build_times():
    """Plot BST build time analysis"""
    try:
        df = pd.read_csv('data/bst_build_times.csv')
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
        
        # Plot 1: Build times with error bars
        ax1.errorbar(df['size'], df['avg_build_time'], 
                    yerr=[df['avg_build_time'] - df['min_build_time'], 
                          df['max_build_time'] - df['avg_build_time']],
                    fmt='bo-', linewidth=2, markersize=8, capsize=5,
                    label='Average Build Time')
        
        # Add theoretical O(n log n) curve
        sizes = df['size'].values
        theoretical = sizes * np.log2(sizes) / 1e7  # Scaled for visibility
        ax1.plot(sizes, theoretical, 'r--', label='O(n log n) scaled', linewidth=2)
        
        ax1.set_xlabel('Number of Nodes (n)')
        ax1.set_ylabel('Build Time (seconds)')
        ax1.set_title('BST Build Time (Cumulative Insertions)')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        ax1.set_xscale('log')
        ax1.set_yscale('log')
        
        # Plot 2: Time per operation
        time_per_op = df['avg_build_time'] / df['size'] * 1e6  # Convert to microseconds
        ax2.plot(df['size'], time_per_op, 'go-', linewidth=2, markersize=8)
        
        ax2.set_xlabel('Number of Nodes (n)')
        ax2.set_ylabel('Time per Insert (μs)')
        ax2.set_title('Average Time per Insert Operation')
        ax2.grid(True, alpha=0.3)
        ax2.set_xscale('log')
        
        plt.tight_layout()
        plt.savefig('graphs/bst_build_times.png', dpi=300, bbox_inches='tight')
        print("✓ Generated: bst_build_times.png")
        
    except FileNotFoundError:
        print("✗ Cannot find bst_build_times.csv - run BST experiments first")
    except Exception as e:
        print(f"✗ Error plotting build times: {e}")

def plot_bst_delete_times():
    """Plot BST delete time analysis"""
    try:
        df = pd.read_csv('data/bst_delete_times.csv')
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
        
        # Plot 1: Delete times
        ax1.errorbar(df['size'], df['avg_delete_time'],
                    yerr=[df['avg_delete_time'] - df['min_delete_time'],
                          df['max_delete_time'] - df['avg_delete_time']],
                    fmt='ro-', linewidth=2, markersize=8, capsize=5,
                    label='Average Delete Time')
        
        ax1.set_xlabel('Number of Nodes (n)')
        ax1.set_ylabel('Total Delete Time (seconds)')
        ax1.set_title('BST Destruction Time (Delete Root Repeatedly)')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        ax1.set_xscale('log')
        ax1.set_yscale('log')
        
        # Plot 2: Compare build vs delete times
        build_df = pd.read_csv('data/bst_build_times.csv')
        ax2.plot(df['size'], df['avg_delete_time'], 'ro-', label='Delete Time', linewidth=2, markersize=8)
        ax2.plot(build_df['size'], build_df['avg_build_time'], 'bo-', label='Build Time', linewidth=2, markersize=8)
        
        ax2.set_xlabel('Number of Nodes (n)')
        ax2.set_ylabel('Time (seconds)')
        ax2.set_title('Build Time vs Delete Time Comparison')
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        ax2.set_xscale('log')
        ax2.set_yscale('log')
        
        plt.tight_layout()
        plt.savefig('graphs/bst_delete_times.png', dpi=300, bbox_inches='tight')
        print("✓ Generated: bst_delete_times.png")
        
    except FileNotFoundError:
        print("✗ Cannot find required CSV files - run BST experiments first")
    except Exception as e:
        print(f"✗ Error plotting delete times: {e}")

def plot_inorder_walk():
    """Plot Inorder-Tree-Walk performance"""
    try:
        df = pd.read_csv('data/inorder_walk_times.csv')
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
        
        # Plot 1: Walk time vs size
        ax1.plot(df['size'], df['avg_walk_time'], 'mo-', linewidth=2, markersize=8, label='Inorder Walk Time')
        
        # Add theoretical O(n) line
        sizes = df['size'].values
        theoretical = sizes / 1e8  # Scaled for visibility
        ax1.plot(sizes, theoretical, 'g--', label='O(n) scaled', linewidth=2)
        
        ax1.set_xlabel('Number of Nodes (n)')
        ax1.set_ylabel('Walk Time (seconds)')
        ax1.set_title('Inorder-Tree-Walk Runtime')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        ax1.set_xscale('log')
        ax1.set_yscale('log')
        
        # Plot 2: Time per node (should be constant for Θ(n))
        time_per_node = df['avg_walk_time'] / df['size'] * 1e9  # Convert to nanoseconds
        ax2.plot(df['size'], time_per_node, 'co-', linewidth=2, markersize=8)
        
        # Add average line
        avg_time = np.mean(time_per_node)
        ax2.axhline(y=avg_time, color='r', linestyle='--', 
                   label=f'Average: {avg_time:.1f} ns/node')
        
        ax2.set_xlabel('Number of Nodes (n)')
        ax2.set_ylabel('Time per Node (nanoseconds)')
        ax2.set_title('Confirming Θ(n) Complexity')
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        ax2.set_xscale('log')
        
        plt.tight_layout()
        plt.savefig('graphs/inorder_walk_times.png', dpi=300, bbox_inches='tight')
        print("✓ Generated: inorder_walk_times.png")
        
    except FileNotFoundError:
        print("✗ Cannot find inorder_walk_times.csv - run BST experiments first")
    except Exception as e:
        print(f"✗ Error plotting inorder walk times: {e}")

def plot_os_comparisons():
    """Plot OS-Tree vs BST comparisons"""
    try:
        # Check if OS-Tree data exists
        if not os.path.exists('data/os_insert_comparison.csv'):
            print("✗ OS-Tree data not found - run OS-Tree experiments first")
            return
            
        insert_df = pd.read_csv('data/os_insert_comparison.csv')
        delete_df = pd.read_csv('data/os_delete_comparison.csv')
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
        
        # Plot 1: Insert comparison
        ax1.plot(insert_df['size'], insert_df['bst_avg_time'], 'bo-', 
                label='BST Insert', linewidth=2, markersize=8)
        ax1.plot(insert_df['size'], insert_df['os_avg_time'], 'ro-', 
                label='OS-Tree Insert', linewidth=2, markersize=8)
        
        ax1.set_xlabel('Number of Nodes (n)')
        ax1.set_ylabel('Build Time (seconds)')
        ax1.set_title('Insert Performance: BST vs OS-Tree')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        ax1.set_xscale('log')
        ax1.set_yscale('log')
        
        # Plot 2: Delete comparison
        ax2.plot(delete_df['size'], delete_df['bst_avg_time'], 'bo-', 
                label='BST Delete', linewidth=2, markersize=8)
        ax2.plot(delete_df['size'], delete_df['os_avg_time'], 'ro-', 
                label='OS-Tree Delete', linewidth=2, markersize=8)
        
        ax2.set_xlabel('Number of Nodes (n)')
        ax2.set_ylabel('Delete Time (seconds)')
        ax2.set_title('Delete Performance: BST vs OS-Tree')
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        ax2.set_xscale('log')
        ax2.set_yscale('log')
        
        plt.tight_layout()
        plt.savefig('graphs/os_tree_comparison.png', dpi=300, bbox_inches='tight')
        print("✓ Generated: os_tree_comparison.png")
        
    except Exception as e:
        print(f"✗ Error plotting OS-Tree comparisons: {e}")

def plot_os_operations():
    """Plot OS-Select and OS-Rank performance"""
    try:
        if not os.path.exists('data/os_operations.csv'):
            print("✗ OS operations data not found - run OS-Tree experiments first")
            return
            
        df = pd.read_csv('data/os_operations.csv')
        
        fig, ax = plt.subplots(figsize=(10, 6))
        
        ax.plot(df['size'], df['select_avg_time'], 'go-', 
               label='OS-Select', linewidth=2, markersize=8)
        ax.plot(df['size'], df['rank_avg_time'], 'mo-', 
               label='OS-Rank', linewidth=2, markersize=8)
        
        # Add theoretical O(log n) reference
        sizes = df['size'].values
        theoretical = np.log2(sizes)  # Scaled appropriately
        ax.plot(sizes, theoretical, 'r--', label='O(log n) scaled', linewidth=2, alpha=0.5)
        
        ax.set_xlabel('Number of Nodes (n)')
        ax.set_ylabel('Time per Operation (microseconds)')
        ax.set_title('OS-Select and OS-Rank Performance')
        ax.legend()
        ax.grid(True, alpha=0.3)
        ax.set_xscale('log')
        
        plt.tight_layout()
        plt.savefig('graphs/os_operations.png', dpi=300, bbox_inches='tight')
        print("✓ Generated: os_operations.png")
        
    except Exception as e:
        print(f"✗ Error plotting OS operations: {e}")

def main():
    """Main function to generate all graphs"""
    print("\n" + "="*50)
    print("BST and OS-Tree Graph Generation")
    print("="*50 + "\n")
    
    ensure_directories()
    
    print("Part A: BST Graphs")
    print("-" * 30)
    plot_bst_heights()
    plot_bst_build_times()
    plot_bst_delete_times()
    plot_inorder_walk()
    
    print("\nPart B: OS-Tree Graphs")
    print("-" * 30)
    plot_os_comparisons()
    plot_os_operations()
    
    print("\n" + "="*50)
    print("Graph generation complete!")
    print("Check the 'graphs' directory for output files")
    print("="*50 + "\n")

if __name__ == "__main__":
    main()