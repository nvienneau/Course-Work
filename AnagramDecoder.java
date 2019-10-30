import java.io.*;
import java.util.*;

public class AnagramDecoder{

	public static void main(String[] args) throws IOException{
		Scanner s;
		StringTable st = new StringTable(9000);
		s = new Scanner(new File(args[0]));
		while(s.hasNext()){
			st.insert(s.next());
		}
		s.close();
		Scanner in = new Scanner(System.in);
		System.out.println("Enter Word you want to find anagrams for:");
		String find = in.next();
		if(st.contains(find)){
			st.getAnagrams(find);
		}
	}
	
	
}