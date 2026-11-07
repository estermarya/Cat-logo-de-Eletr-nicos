const CHAVE = 'catalogoEletronicosProdutos';
const produtosIniciais = [
  { codigo: 1001, nome: 'Smartphone Galaxy A54', categoria: 'Celulares', marca: 'Samsung', preco: 1899.90 },
  { codigo: 1002, nome: 'Notebook Inspiron 15', categoria: 'Computadores', marca: 'Dell', preco: 3499.00 },
  { codigo: 1003, nome: 'Fone Bluetooth QC35', categoria: 'Acessorios', marca: 'Bose', preco: 1299.00 },
  { codigo: 1004, nome: 'Smart TV 50 4K', categoria: 'Outros', marca: 'LG', preco: 2599.90 }
];

let produtos = JSON.parse(localStorage.getItem(CHAVE)) || produtosIniciais;
const lista = document.getElementById('listaProdutos');
const formulario = document.getElementById('formProduto');
const aviso = document.getElementById('aviso');

function salvar() {
  localStorage.setItem(CHAVE, JSON.stringify(produtos));
}

function formatarPreco(valor) {
  return valor.toLocaleString('pt-BR', { style: 'currency', currency: 'BRL' });
}

function mostrarProdutos() {
  const busca = document.getElementById('busca').value.toLowerCase().trim();
  const categoria = document.getElementById('filtroCategoria').value;
  const filtrados = produtos.filter(produto => {
    const texto = `${produto.codigo} ${produto.nome} ${produto.marca}`.toLowerCase();
    return texto.includes(busca) && (categoria === 'Todas' || produto.categoria === categoria);
  });

  lista.innerHTML = '';
  document.getElementById('totalProdutos').textContent = produtos.length;
  document.getElementById('mensagemVazia').style.display = filtrados.length ? 'none' : 'block';

  filtrados.forEach(produto => {
    const linha = document.createElement('tr');
    linha.innerHTML = `
      <td>${produto.codigo}</td>
      <td>${escaparHTML(produto.nome)}</td>
      <td>${escaparHTML(produto.categoria)}</td>
      <td>${escaparHTML(produto.marca)}</td>
      <td class="preco">${formatarPreco(produto.preco)}</td>
      <td>
        <button class="acao" type="button" data-editar="${produto.codigo}">Editar</button>
        <button class="acao excluir" type="button" data-excluir="${produto.codigo}">Excluir</button>
      </td>`;
    lista.appendChild(linha);
  });
}

function escaparHTML(texto) {
  return String(texto).replace(/[&<>"']/g, caractere => ({
    '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;'
  })[caractere]);
}

function limparFormulario() {
  formulario.reset();
  document.getElementById('indiceEdicao').value = '';
  document.getElementById('tituloFormulario').textContent = 'Cadastrar produto';
  document.getElementById('botaoSalvar').textContent = 'Cadastrar produto';
  document.getElementById('cancelarEdicao').classList.add('oculto');
}

formulario.addEventListener('submit', evento => {
  evento.preventDefault();
  const codigo = Number(document.getElementById('codigo').value);
  const produto = {
    codigo,
    nome: document.getElementById('nome').value.trim(),
    categoria: document.getElementById('categoria').value,
    marca: document.getElementById('marca').value.trim(),
    preco: Number(document.getElementById('preco').value)
  };
  const indiceEdicao = document.getElementById('indiceEdicao').value;

  if (!Number.isInteger(codigo) || codigo <= 0 || !produto.nome || !produto.marca || !(produto.preco > 0)) {
    aviso.textContent = 'Confira os dados informados.';
    return;
  }

  const repetido = produtos.some((item, indice) => item.codigo === codigo && String(indice) !== indiceEdicao);
  if (repetido) {
    aviso.textContent = 'Esse código já está cadastrado.';
    return;
  }

  if (indiceEdicao === '') {
    produtos.push(produto);
    aviso.textContent = 'Produto cadastrado com sucesso!';
  } else {
    produtos[Number(indiceEdicao)] = produto;
    aviso.textContent = 'Produto alterado com sucesso!';
  }

  salvar();
  limparFormulario();
  mostrarProdutos();
});

lista.addEventListener('click', evento => {
  const codigoEditar = evento.target.dataset.editar;
  const codigoExcluir = evento.target.dataset.excluir;

  if (codigoEditar) {
    const indice = produtos.findIndex(item => item.codigo === Number(codigoEditar));
    if (indice < 0) return;
    const produto = produtos[indice];
    document.getElementById('codigo').value = produto.codigo;
    document.getElementById('nome').value = produto.nome;
    document.getElementById('categoria').value = produto.categoria;
    document.getElementById('marca').value = produto.marca;
    document.getElementById('preco').value = produto.preco;
    document.getElementById('indiceEdicao').value = indice;
    document.getElementById('tituloFormulario').textContent = 'Alterar produto';
    document.getElementById('botaoSalvar').textContent = 'Salvar alterações';
    document.getElementById('cancelarEdicao').classList.remove('oculto');
    document.getElementById('codigo').focus();
  }

  if (codigoExcluir) {
    const confirmar = window.confirm('Deseja realmente excluir este produto?');
    if (!confirmar) return;
    produtos = produtos.filter(item => item.codigo !== Number(codigoExcluir));
    salvar();
    mostrarProdutos();
    aviso.textContent = 'Produto excluído.';
  }
});

document.getElementById('cancelarEdicao').addEventListener('click', () => {
  limparFormulario();
  aviso.textContent = '';
});
document.getElementById('busca').addEventListener('input', mostrarProdutos);
document.getElementById('filtroCategoria').addEventListener('change', mostrarProdutos);

mostrarProdutos();
