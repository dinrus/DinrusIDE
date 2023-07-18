package org.upp.test;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ScrollView;
import android.widget.TextView;

public class test extends Activity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		ScrollView scrollView = new ScrollView(this);
		TextView textView = new TextView(this);
		
		textView.setText("Привет test!\n");
		
		scrollView.addView(textView);
		setContentView(scrollView);
	}
}
