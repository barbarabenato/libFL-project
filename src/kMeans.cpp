#include "kMeans.h"
#define MAX_n 100000

FeatureMatrix* initializeCentroids(FeatureMatrix* c, int nbins){
	FeatureMatrix* centroids = c;
	for(int i=0; i<centroids->nFeaturesVectors; i++){
		for(int j=0; j<centroids->featureVector[0]->size; j++){
			centroids->featureVector[i]->features[j] = rand() % nbins;
		}	
	}
	return centroids;
}

FeatureVector* initializeCentroid(FeatureVector* c, int nbins){
	FeatureVector* centroids = c;
	for(int j=0; j<centroids->size; j++){
		centroids->features[j] = rand() % nbins;
	}	
	return centroids;
}
/*
FeatureVector* calculateFeatureVectorResult(FeatureVector* v1, FeatureVector* v2){
	FeatureVector* result;
	for(int i=0; i<v1->size; i++){
		result[i] = (v1[i] + v2[i])/2;
	}
	return *result;
	
}*/

//Function to choose the k words to a histogram
FeatureMatrix* computekMeans(FeatureMatrix* histogram, int k, int nbins){
	FeatureMatrix* centroids = NULL;
	FeatureMatrix* old_centroids = NULL;
	int* c, *elements;
	int sizeFeatureVectors = histogram->featureVector[1]->size;
	int dist, min;
	
	elements = (int*) malloc(sizeof(int)*k);
	c = (int*) malloc(sizeof(int)*histogram->nFeaturesVectors);
	
	for(int i=0; i<histogram->nFeaturesVectors; i++)
		c[i]=-1;
	
	centroids = createFeatureMatrix(k, sizeFeatureVectors);
	
	//Initialize the centroids with random values
	centroids = initializeCentroids(centroids, nbins);
	
	while(old_centroids != centroids){
		old_centroids = centroids;
		
		//Atribui para cada histograma, um cluster mais próximo
		for(int i=0; i<histogram->nFeaturesVectors; i++){
			dist = 0;
			min = MAX_n;
			for(int j=0; j<k; j++){		
				dist = pow(vectorDifference(histogram->featureVector[i],centroids->featureVector[j]),2);
				if (dist < min){
					min = dist;
					c[i]=j;
				}
			}
		}
		
		// Zera centroides
		for(int i=0; i<k; i++){
			for(int l=0; l<histogram->featureVector[0]->size; l++)
				centroids->featureVector[i]->features[l] = 0.0;
			elements[i]=0;
		}
		
		//Atualiza centroides
		for(int j=0; j<histogram->nFeaturesVectors; j++){
			for(int l=0; l<histogram->featureVector[0]->size; l++){
				centroids->featureVector[c[j]]->features[l] += histogram->featureVector[j]->features[l];		
			}
			elements[c[j]]++;
		}
		for(int i=0; i<k; i++){
			if(elements[i]!=0){
				for(int l=0; l<histogram->featureVector[0]->size; l++)
					centroids->featureVector[i]->features[l] = centroids->featureVector[i]->features[l]/elements[i];
			}
			else{//Se centroide não tiver nenhum elemento
				centroids->featureVector[i]=initializeCentroid(centroids->featureVector[i],nbins);
			}
		}
	}
	
	free(elements);
	free(c);
	destroyFeatureMatrix(&old_centroids);
	
	return centroids;
}
