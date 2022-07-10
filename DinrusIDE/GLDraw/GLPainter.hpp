template <typename Src>
void GLPolygons(GLVertexData& mesh, const Src& polygon)
{
	Вектор<ТочкаПЗ> vertex;
	Вектор<int> ndx;
	
	mesh.очисть();
	
	{
		GL_TIMING("GLPolygons");
		for(const auto& p: polygon) {
			int i0 = vertex.дайСчёт();
			for(int i = 0; i < p.дайСчёт(); i++) {
				if(i > 1)
					ndx << i0 << i0 + i - 1 << i0 + i;
				vertex.добавь(p[i]);
			}
		}
	}
	
	mesh.добавь(vertex).Индекс(ndx);
}

template <typename Src>
void GLPolylines(GLVertexData& data, const Src& polygon, bool close)
{
	Вектор<float> vertex;
	Вектор<int> ndx;
	
	{
		GL_TIMING("GLPolylines");
		int ii = 0;
		for(const auto& p: polygon) {
			int i0 = vertex.дайСчёт();
			int ii0 = ii;
			int m = 1;
			
			if(p.дайСчёт() && close && p[0] != p.верх())
				m = 0;
			for(int i = 0; i < p.дайСчёт() - m; i++) {
				ТочкаПЗ p1 = p[i];
				ТочкаПЗ p2 = p[i + 1 < p.дайСчёт() ? i + 1 : 0];
				ТочкаПЗ un = p1 - p2;
				
				vertex << (float)p1.x << (float)p1.y << (float)un.x << (float)un.y;
				vertex << (float)p1.x << (float)p1.y << -(float)un.x << -(float)un.y;
	
				vertex << (float)p2.x << (float)p2.y << (float)un.x << (float)un.y;
				vertex << (float)p2.x << (float)p2.y << -(float)un.x << -(float)un.y;
	
				ndx << ii << ii + 1 << ii + 2
				    << ii + 3 << ii + 2 << ii + 1;
				
				if(i) // if line is not first, draw bevel join between current and previous line
					ndx << ii << ii + 1 << ii - 4 + 3
					    << ii - 4 + 2 << ii - 4 + 3 << ii;
	
				ii += 4;
			}
	
			if(p.дайСчёт() > 2 && (p.верх() == p[0] || close)) // Строка loop is closed, draw bevel join
				ndx << ii0 << ii0 + 1 << ii - 4 + 3
				    << ii - 4 + 2 << ii - 4 + 3 << ii0;
		}
	}
	
	data.добавь(vertex, 4).Индекс(ndx);
}
