public class App{
    //Primeira Seção -> Verificação das coordenadas onde é igual a 1
    record Coordenada(int id,int linha, int coluna) {}
    public static Coordenada[] buscaCoor(int imagem[][],int rows,int colms){
        Coordenada coor[]=new Coordenada[colms*rows];
        int count=0;
        for(int i=0;i<rows;i++){
            for(int j=0;j<colms;j++){
                if(imagem[i][j]==1){
                    coor[count]=new Coordenada(count,i,j);
                    count++;
                }
            }
        }
        return coor;
    }
    //Segunda Seção -> Criar Parent e Rank para cada coordenada
    public static UnionFind[] criaVetores(Coordenada coor[]) {
        UnionFind[] uf = new UnionFind[coor.length];
        for(int i = 0; i < coor.length; i++) {
            if(coor[i] != null) {
                uf[i] = new UnionFind(coor[i].id);
            }
        }
        return uf;
    }
    //Terceira Seção -> Identificar via Varredura ligações 
    //Quarta Seção -> Union dirá a quantidade de figuras
    public static void main(String[]args){
        int[][] imagem=new int[4][4];
        Coordenada coor[]=buscaCoor(imagem,4,4);
        UnionFind[] unionfind=criaVetores(coor);
    }
}