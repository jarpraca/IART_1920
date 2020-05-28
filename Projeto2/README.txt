Pré-requisitos:
  * Instalação do anaconda ou do pip 
  * Instalação do jupyter notebook

No diretório onde se encontra o ficheiro IART_2.ipynb:

  * Caso o setup não esteja efetuado com o anaconda, é necessário correr os seguintes comandos:
      -> pip install matplotlib
      -> pip install sklearn
      -> pip install pandas
      -> pip install numpy
      -> pip install statsmodels
      -> pip install sqlite3
  * Correr o comando " pip install missingno" ou "conda install missingno" (opcional) - Este import é utilizado para a visualização dos dados omissos no dataset. O import está inicialmente comentado para que seja possível visualizar tudo sem problemas. Caso queira ativar esta funcionalidade é apenas necessário descomentar o import e colocar a variável display_nan_graph a True 
  * Correr o comando jupyter notebook na command line /terminal
  * Abrir o ficheiro IART_2.ipynb
  * Correr todas as cells