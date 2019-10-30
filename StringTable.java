import java.util.*;


public class StringTable{
	
	private int tableSize;
	private Node[] st;
	
	private static class Node{
		private String key;
		private String value;
		private Node next;
		
		public Node(String key,String value,Node next){
			this.key = key;
			this.value = value;
			this.next = next;
		}
		
		public String toString(){
			return value;
		}
	}
	
	public StringTable(int tableSize){
		this.tableSize = tableSize;
		st = new Node[tableSize];
	}
	
	public void insert(String value){
		String key = sortString(value);
		int index = hash(key);
		if(value.equals("cat")) {
			1+1;
		}
		
		if(st[index] != null){
			Node temp = st[index];
			while(temp != null){
				temp = temp.next;
			}
			temp = new Node(key,value,null);
		}else{
			st[index] = new Node(key,value,null);
		}
	}
	
	public boolean contains(String value){
		String key = sortString(value);
		int index = hash(key);
		if(st[index] != null){
			return true;
		}else{
			return false;
		}
	}

	private int hash(String key){
		int hashValue = 0;
		
		for(int i = 0;i<key.length();i++){
			hashValue = (31*hashValue+key.charAt(i)) % this.tableSize;
		}
	
		return hashValue;
	}
	
	private String sortString(String s){
		char tempArray[] = s.toLowerCase().toCharArray();
		Arrays.sort(tempArray);
		return new String(tempArray);
	}
	
	public void getAnagrams(String value){
		String key = sortString(value);
		System.out.println(key);
		int index = hash(key);
		Node start = st[index];
		System.out.println(index);
		while(start != null){
			System.out.println
			start = start.next;
		}
		
	}
	
	
	
}