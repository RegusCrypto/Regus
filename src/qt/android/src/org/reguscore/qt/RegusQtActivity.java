package org.reguscore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class RegusQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File regusDir = new File(getFilesDir().getAbsolutePath() + "/.regus");
        if (!regusDir.exists()) {
            regusDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
