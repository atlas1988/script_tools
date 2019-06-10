package com.example.atlas.alcohol;

//import com.example.atlas.alcohol.R;
//import android.support.v7.app.AppCompatActivity;
import android.app.Activity;
import android.os.Bundle;
import android.os.ServiceManager;
import android.os.IAlcoholService;
import android.os.RemoteException;
import android.util.Log;
import android.util.EventLog;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.textservice.TextServicesManager;
import android.widget.Button;
import android.widget.EditText;
import java.io.IOException;
import java.util.ArrayList;

import android.app.ListActivity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.drawable.Drawable;

public class alcohol extends Activity implements  OnClickListener {
    private final static String LOG_TAG = "atlas.alcohol";
    private IAlcoholService alcoholService = null;
    private EditText valueText = null;
    private Button readButton = null;
    private Button writeButton = null;
    private Button clearButton = null;

    // called when the activity is first created.
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alcohol);
        alcoholService = IAlcoholService.Stub.asInterface(ServiceManager.getService("alcohol"));
        valueText = (EditText)findViewById(R.id.edit_value);
        readButton = (Button)findViewById(R.id.button_read);
        writeButton = (Button)findViewById(R.id.button_write);
        clearButton = (Button)findViewById(R.id.button_clear);

        readButton.setOnClickListener(this);
        writeButton.setOnClickListener(this);
        clearButton.setOnClickListener(this);

        Log.i(LOG_TAG,"Alcohol Activity Created");
    }
    @Override
    public void onClick(View v){
        if(v.equals(readButton)){
            try{
                float val = alcoholService.getVal();
                String text = String.valueOf(val);
                valueText.setText(text);
            }catch (RemoteException e){
                Log.e(LOG_TAG,"Remote Exception while reading value from device.");
            }
        }else if(v.equals(writeButton)){
            try{
                String text = valueText.getText().toString();
                int val = Integer.parseInt(text);
                alcoholService.setVal(val);
            }catch (RemoteException e){
                Log.e(LOG_TAG,"Remote Exception while writing value to device.");
            }
        }else if(v.equals(clearButton)){
            String text = "";
            valueText.setText(text);
        }
    }
}
