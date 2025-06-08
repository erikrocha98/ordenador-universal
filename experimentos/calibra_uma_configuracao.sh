#!/bin/bash
# experimento_uma_config.sh
# Script completo para UMA configuração: compila, coleta e calibra

# Configuração FIXA (edite aqui para mudar)
KEYSZ=4
PLSZ=20
TIPO=aleatorio  # Não usado na coleta, mas importante para análise

# Parâmetros da coleta
MIN_TAM=1000
MAX_TAM=20000
NUM_PONTOS=40

echo "=== EXPERIMENTO PARA UMA CONFIGURAÇÃO ==="
echo "Configuração FIXA:"
echo "  - KEYSZ = $KEYSZ bytes"
echo "  - PLSZ = $PLSZ bytes"
echo "Variando apenas: tamanho do vetor ($MIN_TAM a $MAX_TAM)"
echo ""

# Criar diretórios
mkdir -p bin/experimentos
mkdir -p resultados/{medicoes,coeficientes,graficos}

# ETAPA 1: COMPILAÇÃO
echo "=== ETAPA 1: COMPILAÇÃO ==="
echo -n "Compilando programa de coleta... "

g++ -O2 -std=c++11 -DKEYSZ=$KEYSZ -DPLSZ=$PLSZ \
    coleta_medicoes.cpp \
    -o bin/experimentos/coleta_k${KEYSZ}_p${PLSZ} \
    -lm 2> /tmp/erro_compilacao.log

if [ $? -eq 0 ]; then
    echo "OK"
else
    echo "ERRO"
    echo "Detalhes do erro:"
    cat /tmp/erro_compilacao.log
    exit 1
fi

# ETAPA 2: COLETA DE DADOS
echo ""
echo "=== ETAPA 2: COLETA DE DADOS ==="
echo "Executando coleta (isso pode demorar alguns minutos)..."
echo ""

# Executar o programa de coleta
./bin/experimentos/coleta_k${KEYSZ}_p${PLSZ} $MIN_TAM $MAX_TAM $NUM_PONTOS

# Verificar se foram gerados arquivos
if ls medicoes_k${KEYSZ}_p${PLSZ}_*.txt 1> /dev/null 2>&1; then
    echo ""
    echo "Arquivos gerados:"
    ls -la medicoes_k${KEYSZ}_p${PLSZ}_*.txt
    
    # Mover para diretório de resultados
    mv medicoes_k${KEYSZ}_p${PLSZ}_*.txt resultados/medicoes/
    echo ""
    echo "Arquivos movidos para resultados/medicoes/"
else
    echo "ERRO: Nenhum arquivo de medição foi gerado!"
    exit 1
fi

# ETAPA 3: CALIBRAÇÃO DOS COEFICIENTES
echo ""
echo "=== ETAPA 3: CALIBRAÇÃO DOS COEFICIENTES ==="

# ...existing code...

# Modificar para apenas tipo aleatório
cd resultados/medicoes/

echo ""
tipo="aleatorio"  # Only process random type
arquivo="medicoes_k${KEYSZ}_p${PLSZ}_${tipo}.txt"
    
if [ -f "$arquivo" ]; then
    echo "----------------------------------------"
    echo "Calibrando para tipo: $tipo"
    echo ""
    
    # Mostrar preview dos dados
    echo "Preview dos dados:"
    head -5 $arquivo
    echo "..."
    tail -2 $arquivo
    echo ""
    
    # Calibrar
    python3 ../../calibra_coeficientes.py $KEYSZ $PLSZ $tipo
    
    # Mover resultados
    if [ -f "coef_k${KEYSZ}_p${PLSZ}_${tipo}.txt" ]; then
        mv coef_k${KEYSZ}_p${PLSZ}_${tipo}.txt ../coeficientes/
        
        # Mostrar coeficientes
        echo ""
        echo "Coeficientes obtidos:"
        echo -n "  a (chamadas) = "
        sed -n '5p' ../coeficientes/coef_k${KEYSZ}_p${PLSZ}_${tipo}.txt
        echo -n "  b (comparações) = "
        sed -n '6p' ../coeficientes/coef_k${KEYSZ}_p${PLSZ}_${tipo}.txt
        echo -n "  c (movimentações) = "
        sed -n '7p' ../coeficientes/coef_k${KEYSZ}_p${PLSZ}_${tipo}.txt
    fi
    
    if [ -f "calibracao_k${KEYSZ}_p${PLSZ}_${tipo}.png" ]; then
        mv calibracao_k${KEYSZ}_p${PLSZ}_${tipo}.png ../graficos/
    fi
fi

cd ../..

# ETAPA 4: RESUMO
echo ""
echo "=== RESUMO DO EXPERIMENTO ==="
echo ""
echo "Configuração testada: KEYSZ=$KEYSZ, PLSZ=$PLSZ"
echo ""
echo "Arquivos de medição em: resultados/medicoes/"
ls -la resultados/medicoes/medicoes_k${KEYSZ}_p${PLSZ}_*.txt 2>/dev/null | awk '{print "  - " $9}'

echo ""
echo "Coeficientes calibrados em: resultados/coeficientes/"
ls -la resultados/coeficientes/coef_k${KEYSZ}_p${PLSZ}_*.txt 2>/dev/null | awk '{print "  - " $9}'

echo ""
echo "Gráficos em: resultados/graficos/"
ls -la resultados/graficos/calibracao_k${KEYSZ}_p${PLSZ}_*.png 2>/dev/null | awk '{print "  - " $9}'

# Criar arquivo resumo
cat > resultados/resumo_k${KEYSZ}_p${PLSZ}.txt << EOF
RESUMO DO EXPERIMENTO
Data: $(date)

Configuração FIXA:
  KEYSZ = $KEYSZ bytes
  PLSZ = $PLSZ bytes

Variação:
  Tamanho do vetor: $MIN_TAM a $MAX_TAM ($NUM_PONTOS pontos)

Coeficientes calibrados (μs):
EOF

# Adicionar coeficientes ao resumo
tipo="aleatorio"
coef_file="resultados/coeficientes/coef_k${KEYSZ}_p${PLSZ}_${tipo}.txt"
if [ -f "$coef_file" ]; then
    echo "" >> resultados/resumo_k${KEYSZ}_p${PLSZ}.txt
    echo "Tipo: $tipo" >> resultados/resumo_k${KEYSZ}_p${PLSZ}.txt
    echo "  a = $(sed -n '5p' $coef_file)" >> resultados/resumo_k${KEYSZ}_p${PLSZ}.txt
    echo "  b = $(sed -n '6p' $coef_file)" >> resultados/resumo_k${KEYSZ}_p${PLSZ}.txt
    echo "  c = $(sed -n '7p' $coef_file)" >> resultados/resumo_k${KEYSZ}_p${PLSZ}.txt
    r2=$(grep "R²" $coef_file | cut -d'=' -f2)
    echo "  R² = $r2" >> resultados/resumo_k${KEYSZ}_p${PLSZ}.txt
fi


echo ""
echo "Resumo salvo em: resultados/resumo_k${KEYSZ}_p${PLSZ}.txt"
echo ""
echo "=== EXPERIMENTO CONCLUÍDO ==="
echo ""
echo "Para testar outra configuração:"
echo "1. Edite as variáveis KEYSZ e PLSZ no início deste script"
echo "2. Execute novamente"