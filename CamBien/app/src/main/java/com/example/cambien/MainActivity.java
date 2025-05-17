package com.example.cambien;

import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    private TextView txtNhietDo, txtDoAm, txtTrangThaiLed;
    private Button btnOn, btnOff;
    private DatabaseReference database;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        txtNhietDo = findViewById(R.id.txtNhietDo);
        txtDoAm = findViewById(R.id.txtDoAm);
        txtTrangThaiLed = findViewById(R.id.txtTrangThaiLed); // ánh xạ trạng thái đèn
        btnOn = findViewById(R.id.btnOn);
        btnOff = findViewById(R.id.btnOff);

        // Tham chiếu database gốc
        database = FirebaseDatabase.getInstance().getReference();

        // Cập nhật nhiệt độ và độ ẩm khi có thay đổi
        database.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if(snapshot.exists()) {
                    Long nhietDo = snapshot.child("NhietDo").getValue(Long.class);
                    Long doAm = snapshot.child("DoAm").getValue(Long.class);
                    Long ledStatus = snapshot.child("led").getValue(Long.class);

                    if (nhietDo != null) {
                        txtNhietDo.setText("Nhiệt độ: " + nhietDo + "°C");
                    }

                    if (doAm != null) {
                        txtDoAm.setText("Độ ẩm: " + doAm + "%");
                    }

                    if (ledStatus != null) {
                        if (ledStatus == 1) {
                            txtTrangThaiLed.setText("Trạng thái LED: Bật");
                        } else {
                            txtTrangThaiLed.setText("Trạng thái LED: Tắt");
                        }
                    } else {
                        txtTrangThaiLed.setText("Trạng thái LED: --");
                    }
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                txtNhietDo.setText("Lỗi đọc nhiệt độ");
                txtDoAm.setText("Lỗi đọc độ ẩm");
                txtTrangThaiLed.setText("Lỗi đọc trạng thái LED");
            }
        });

        // Điều khiển LED
        DatabaseReference ledRef = database.child("led");
        btnOn.setOnClickListener(v -> ledRef.setValue(1));
        btnOff.setOnClickListener(v -> ledRef.setValue(0));
    }
}
