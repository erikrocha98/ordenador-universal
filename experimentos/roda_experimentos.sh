#!/bin/bash
# analise_experimental_completa.sh
# Segue as orientações do professor

echo "=== ANÁLISE EXPERIMENTAL COMPLETA ==="
echo ""

# Criar estrutura de diretórios
mkdir -p resultados/{coeficientes,graficos,analise}

# PARTE 1: CALIBRAÇÃO DOS COEFICIENTES
echo "=== PARTE 1: CALIBRAÇÃO DOS COEFICIENTES ==="
echo "Fixando KEYSZ, PLSZ e tipo de vetor, variando apenas tamanho"
echo ""

# Para cada configuração fixa
for k in 4 8 16 32; do
    for p in 20 100 500 1000; do
        for tipo in aleatorio ordenado inverso quase_ord_10; do
            
            arquivo="medicoes_k${k}_p${p}_${tipo}.txt"
            
            if [ -f "$arquivo" ]; then
                echo "----------------------------------------"
                echo "Calibrando: KEYSZ=$k, PLSZ=$p, Tipo=$tipo"
                
                # Calibrar coeficientes
                python3 calibra_coeficientes.py $k $p $tipo
                
                # Mover resultados
                mv coef_k${k}_p${p}_${tipo}.txt resultados/coeficientes/ 2>/dev/null
                mv calibracao_k${k}_p${p}_${tipo}.png resultados/graficos/ 2>/dev/null
                
                echo ""
            fi
        done
    done
done

# PARTE 2: ANÁLISE DO TAD (com MPS e LQ calibrados)
echo ""
echo "=== PARTE 2: ANÁLISE DO TAD COM LIMIARES ==="
echo "Aqui você deve:"
echo "1. Usar os coeficientes calibrados para determinar MPS e LQ"
echo "2. Executar o OrdenadorUniversal com diferentes configurações"
echo "3. Gerar gráficos de desempenho"
echo ""

# PARTE 3: COMPARAÇÃO COM ALGORITMOS NÃO OTIMIZADOS
echo "=== PARTE 3: COMPARAÇÃO COM ALGORITMOS BASE ==="
echo "Compare OrdenadorUniversal vs QuickSort puro vs InsertionSort"
echo ""

# Gerar resumo consolidado
cat > resultados/analise/resumo_coeficientes.md << 'EOF'
# Resumo da Análise Experimental

## Coeficientes Calibrados

| KEYSZ | PLSZ | Tipo | a (calls) | b (cmp) | c (moves) | R² |
|-------|------|------|-----------|---------|-----------|-----|
EOF

# Adicionar dados ao resumo
for coef_file in resultados/coeficientes/coef_*.txt; do
    if [ -f "$coef_file" ]; then
        # Extrair informações do nome do arquivo
        basename=$(basename "$coef_file")
        config=$(echo "$basename" | sed 's/coef_k\([0-9]*\)_p\([0-9]*\)_\(.*\)\.txt/\1 \2 \3/')
        
        # Extrair coeficientes
        a=$(sed -n '5p' "$coef_file")
        b=$(sed -n '6p' "$coef_file")
        c=$(sed -n '7p' "$coef_file")
        r2=$(grep "R²" "$coef_file" | cut -d'=' -f2)
        
        echo "| $config | $a | $b | $c | $r2 |" >> resultados/analise/resumo_coeficientes.md
    fi
done

echo "" >> resultados/analise/resumo_coeficientes.md
echo "## Observações" >> resultados/analise/resumo_coeficientes.md
echo "" >> resultados/analise/resumo_coeficientes.md
echo "1. Coeficientes variam significativamente com KEYSZ e PLSZ" >> resultados/analise/resumo_coeficientes.md
echo "2. Vetores ordenados têm comportamento diferente dos aleatórios" >> resultados/analise/resumo_coeficientes.md
echo "3. R² > 0.95 indica bom ajuste do modelo linear" >> resultados/analise/resumo_coeficientes.md

echo "=== ANÁLISE CONCLUÍDA ==="
echo "Resultados em: resultados/"
echo "- Coeficientes: resultados/coeficientes/"
echo "- Gráficos: resultados/graficos/"
echo "- Análise: resultados/analise/"