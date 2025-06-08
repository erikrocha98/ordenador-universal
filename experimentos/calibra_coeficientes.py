#!/usr/bin/env python3
"""
Calibração de coeficientes para cada configuração fixa
Conforme orientações: fixa KEYSZ, PLSZ e tipo de vetor
"""

import numpy as np
import pandas as pd
from scipy.optimize import nnls
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt
import sys
import os

def calibrar_configuracao_especifica(keysz, plsz, tipo_vetor):
    """
    Calibra a, b, c para uma configuração específica
    """
    arquivo = f'medicoes_k{keysz}_p{plsz}_{tipo_vetor}.txt'
    
    print(f"\n{'='*60}")
    print(f"CALIBRAÇÃO PARA CONFIGURAÇÃO FIXA:")
    print(f"KEYSZ={keysz}, PLSZ={plsz}, Tipo={tipo_vetor}")
    print(f"{'='*60}")
    
    try:
        # Ler dados
        df = pd.read_csv(arquivo, sep=r'\s+', comment='#',
                        names=['tamanho', 'comparacoes', 'movimentacoes', 'chamadas', 'tempo'])
        print(f"Dados carregados: {len(df)} tamanhos de vetor diferentes")
        
    except FileNotFoundError:
        print(f"ERRO: Arquivo {arquivo} não encontrado!")
        return None
    
    # Mostrar dados coletados
    print("\nDados coletados (variando apenas tamanho do vetor):")
    print(df.to_string(index=False))
    
    # Preparar sistema de equações
    # T_i = a*cmp_i + b*moves_i + c*calls_i
    
    # Matriz X: cada linha é [calls_i, cmp_i, moves_i]
    X = df[['comparacoes', 'movimentacoes', 'chamadas']].values
    
    # Vetor y: tempos medidos
    y = df['tempo'].values.astype(float)

    # Normalizar dados
    scaler_X = StandardScaler()
    scaler_y = StandardScaler()
    
    X_norm = scaler_X.fit_transform(X)
    y_norm = scaler_y.fit_transform(y.reshape(-1, 1)).ravel()
    
    print("\n--- REGRESSÃO SIMPLES ---")
    print("Sistema: T = a*cmp + b*moves + c*calls")
    
    # Usar regressão linear simples
    reg = LinearRegression(fit_intercept=True)
    reg.fit(X_norm, y_norm)
    
    # Desnormalizar coeficientes
    stds_X = scaler_X.scale_
    std_y = scaler_y.scale_
    means_X = scaler_X.mean_
    mean_y = scaler_y.mean_ 
    
    coef = reg.coef_ * std_y / stds_X
    intercept = mean_y - np.sum(coef * means_X)
    
    a, b, c = coef

    # Calcular R²
    y_pred = X @ coef
    ss_res = np.sum((y - y_pred) ** 2)
    ss_tot = np.sum((y - np.mean(y)) ** 2)
    r2 = 1 - (ss_res / ss_tot) if ss_tot > 0 else 0
    
    print(f"\nCoeficientes (NNLS):")
    print(f"a (tempo por chamada):      {a:.9e} μs")
    print(f"b (tempo por comparação):   {b:.9e} μs")
    print(f"c (tempo por movimentação): {c:.9e} μs")
    print(f"R² = {r2:.6f}")
    
    # Análise de custo
    print("\n--- ANÁLISE DE CUSTO ---")
    print(f"Custo relativo:")
    if b > 0:
        print(f"- Uma movimentação custa {c/b:.2f}x uma comparação")
    if a > 0:
        print(f"- Uma chamada custa {a/b:.2f}x uma comparação")
    
    # Validação do modelo
    print("\n--- VALIDAÇÃO DO MODELO ---")
    print("Tamanho | Tempo Real | Tempo Previsto | Erro %")
    print("-" * 50)
    
    for i in range(len(df)):
        t_real = y[i]
        t_prev = y_pred[i]
        erro = abs(t_real - t_prev) / t_real * 100 if t_real > 0 else 0
        print(f"{df.iloc[i]['tamanho']:7d} | {t_real:10.1f} | {t_prev:14.1f} | {erro:6.1f}%")
    
    # Gerar gráfico
    plt.figure(figsize=(10, 6))
    
    # Subplot 1: Tempo vs Tamanho
    plt.subplot(1, 2, 1)
    plt.scatter(df['tamanho'], y, label='Real', alpha=0.7)
    plt.plot(df['tamanho'], y_pred, 'r-', label='Modelo', linewidth=2)
    plt.xlabel('Tamanho do Vetor')
    plt.ylabel('Tempo (μs)')
    plt.title('Tempo Real vs Modelo')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Subplot 2: Resíduos
    plt.subplot(1, 2, 2)
    residuos = y - y_pred
    plt.scatter(df['tamanho'], residuos, alpha=0.7)
    plt.axhline(y=0, color='r', linestyle='--')
    plt.xlabel('Tamanho do Vetor')
    plt.ylabel('Resíduo (Real - Previsto)')
    plt.title('Análise de Resíduos')
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()
    nome_grafico = f'calibracao_k{keysz}_p{plsz}_{tipo_vetor}.png'
    plt.savefig(nome_grafico, dpi=150)
    print(f"\nGráfico salvo em: {nome_grafico}")
    
    # Salvar coeficientes
    nome_saida = f'coef_k{keysz}_p{plsz}_{tipo_vetor}.txt'
    with open(nome_saida, 'w') as f:
        f.write(f"# Coeficientes calibrados\n")
        f.write(f"# KEYSZ={keysz} PLSZ={plsz} Tipo={tipo_vetor}\n")
        f.write(f"# R²={r2:.6f}\n")
        f.write(f"# a (cmp), b (moves), c (calls)\n")
        f.write(f"{a:.9e}\n")
        f.write(f"{b:.9e}\n")
        f.write(f"{c:.9e}\n")
    
    return {
        'keysz': keysz,
        'plsz': plsz,
        'tipo': tipo_vetor,
        'a': a,
        'b': b,
        'c': c,
        'r2': r2
    }

def main():
    if len(sys.argv) < 4:
        print("Uso: python3 calibra_por_configuracao.py <KEYSZ> <PLSZ> <tipo_vetor>")
        print("Exemplo: python3 calibra_por_configuracao.py 8 100 aleatorio")
        print("\nTipos disponíveis: aleatorio, ordenado, inverso, quase_ord_10, quase_ord_25, quase_ord_50")
        sys.exit(1)
    
    keysz = int(sys.argv[1])
    plsz = int(sys.argv[2])
    tipo = sys.argv[3]
    
    resultado = calibrar_configuracao_especifica(keysz, plsz, tipo)
    
    if resultado:
        print(f"\n{'='*60}")
        print("RESUMO FINAL:")
        print(f"Configuração: KEYSZ={keysz}, PLSZ={plsz}, Tipo={tipo}")
        print(f"Coeficientes calibrados com R²={resultado['r2']:.4f}:")
        print(f"  a = {resultado['a']:.6e} μs/comparação")
        print(f"  b = {resultado['b']:.6e} μs/movimentação")
        print(f"  c = {resultado['c']:.6e} μs/chamada")
        print(f"{'='*60}")

if __name__ == "__main__":
    main()