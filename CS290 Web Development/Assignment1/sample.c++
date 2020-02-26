int minPrice(vector<vector<int>> cost)
{
       vector<int> result;
       vector<int> prevLoc;
       vector<vector<int>>::const_iterator row;
       vector<int>::const_iterator col;

        int prevLocation = 0;

        for(row = cost.begin(); row != cost.end(); row++)
        {
            int rowTemp = 999;
            int currentLoc = 0;
            for(col = row->begin(); col != row->end(); col++)
            {               
                if( *col < rowTemp)
                {
                    if(result.size() == 0)
                    {
                        rowTemp = *col;
                        prevLoc.push_back(currentLoc);
                    }
                    
                    if( result.size() != 0 &&currentLoc != prevLoc.back())
                    {
                        rowTemp = *col;
                        prevLoc.push_back(currentLoc);
                    }

                    currentLoc++;
                }
                //cout<< *col;
            }
            cout << "rowTemp is " << rowTemp << " " ;
            result.push_back(rowTemp);

            cout << endl;
        }

        int finalResult = 0;
        for(int i = 0 ; i < result.size();i++)
        {
            finalResult = finalResult + result[i];
            //cout << "result[i] is "<< result[i] << endl;
            //cout << "
        }
        return finalResult;
}